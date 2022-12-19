#include "game.h"
// https://www.cs.cornell.edu/~adith/docs/y_hex.pdf
// https://webdocs.cs.ualberta.ca/~hayward/papers/m2.pdf

random_device rd;
default_random_engine rng(rd());

/**
 * @brief 
 * 
 * @param game 
 * @param board_size 
 */
void init_game(Game* game, int board_size) {

    // Create board size
    game->board_size = board_size;
    game->move_cnt = 0;

    if (board_size > MAX_BOARD_SIZE) {
        cout << "ERROR: Board size must be less than " << MAX_BOARD_SIZE << endl;
        exit(1);
    }

    for (int i = 0; i < game->board_size * game->board_size; i++) {
        game->board[i] = EMPTY;
    }

    game->last_move = -1;
    game->move_cnt = 0;

    // Initialize neighbours
    init_neighbours(game);

    return;
}

/**
 * @brief Precomputes the neighbours of each valid position on the board
 * 
 * @param game The game object in which to store the neighbours.
 */
void init_neighbours(Game* game) {
    game->neighbours.clear();

    vector<int> offsets_normal = {-1, 1, -game->board_size, game->board_size, -game->board_size+1, game->board_size-1 };
    vector<int> offsets_left   = {    1, -game->board_size, game->board_size, -game->board_size+1                    };
    vector<int> offsets_right  = {-1,    -game->board_size, game->board_size,                     game->board_size-1 };
    vector<int> offsets;

    for (int i = 0; i < game->board_size * game->board_size; i++ ) {
        game->neighbours.push_back(vector<int>());
        if ((i+1) % game->board_size == 0)
            offsets = offsets_right;
        else if (i % game->board_size == 0)
            offsets = offsets_left;
        else
            offsets = offsets_normal;

        for (auto& offset : offsets) {
            if (0 <= i + offset && i + offset < game->board_size * game->board_size)
                game->neighbours.back().push_back(i + offset);
        }
    }

    return;
}

/**
 * @brief Prints a human-readable state of the board to stdout
 * 
 * @param game The current state of the game.
 */
void show_board(Game* game) {
    int space_cnt = 0;

    for (int i = 0; i < game->board_size * game->board_size; i++) {
        if (i % game->board_size == 0) {
            cout << endl;
            space_cnt++;
            for (int _ = 0; _ < space_cnt; _++)
                cout << " ";
        }
        if (game->board[i] == WHITE)
            cout << "W ";
        else if (game->board[i] == BLACK)
            cout << "B ";
        else
            cout << ". ";
    }

    cout << endl;
}

/**
 * @brief Swaps the colors of the two players. This can only be done on the first move
 * of the opponent.
 * 
 * @param game The current instance of the game.
 */
void swap(Game* game) {
    if (game->move_cnt != 1)
        return;

    TYPES t = game->own_color;
    game->own_color = game->opp_color;
    game->opp_color = t;
    game->move_cnt++;

    return;
}

/**
 * @brief 
 * 
 * @param game 
 * @param move 
 */
void unset(Game* game, string move) {
    int coord = str_to_move(game, move);
    game->board[coord] = EMPTY;
    return;
}

/**
 * @brief 
 * 
 * @param game 
 * @param color 
 * @return true 
 * @return false 
 */
TYPES check_win(Game* game, bool print) {
    // Early termination if the game cannot be over
    if (game->move_cnt < game->board_size) {
        if (print)
            cout << "0" << endl;

        return EMPTY;
    }

    vector<bool> seen(game->board_size * game->board_size, false);
    vector<int>::size_type pos = 0;
    vector<int> bfs;
    bfs.reserve(pow(game->board_size, 2));
    TYPES current_color;
    int minPos, maxPos, minModPos, maxModPos;
    int low = 0, high = pow(game->board_size, 2);

    for (int move = low; move < high; move++) {
        // TODO: Establish which direction to look
        if (game->board[move] != EMPTY && !seen[move]) {
            current_color = game->board[move];
            bfs.push_back(move);
            seen[move] = true;
 
            // Get the best locations of the current path
            minPos = move, maxPos = move;
            minModPos = move % game->board_size, maxModPos = move % game->board_size;

            for (; pos < bfs.size(); pos++) {
                move = bfs[pos];
                for (auto& next_move : game->neighbours[move]) {
                    if (game->board[next_move] == current_color && !seen[next_move]) {

                        minPos = min(minPos, next_move);
                        minModPos = min(minModPos, next_move % game->board_size);
                        maxPos = max(maxPos, next_move);
                        maxModPos = max(maxModPos, next_move % game->board_size);

                        // Win conditions
                        // White touches left & right sides
                        // Black touches top & bottom
                        if (current_color == WHITE && minModPos == 0 && maxModPos == game->board_size - 1) {
                            if (print)
                                cout << (game->own_color == WHITE ? 1 : -1) << endl;
                            return WHITE;
                        } else if (current_color == BLACK && minPos < game->board_size && game->board_size * (game->board_size - 1) <= maxPos) {
                            if (print)
                                cout << (game->own_color == BLACK ? 1 : -1) << endl;
                            return BLACK;
                        }

                        bfs.push_back(next_move);
                        seen[next_move] = true;
                    }
                }
            }
        } 
    }

    // No winner yet
    if (print)
        cout << "0" << endl;

    return EMPTY;
}

/**
 * @brief Performs the actual MCTS search
 * 
 * @param game 
 */
void make_move(Game* game) {
    // Perform the MCTS search
    int best_move = 0;
    int depth = 0;
    int best_depth = 0;
    int board_size = pow(game->board_size, 2);
    TYPES to_move = game->own_color;
    Game game_copy = *game;

    vector<int>::size_type white_moves_idx;
    vector<int>::size_type black_moves_idx;
    vector<int> white_moves;
    vector<int> black_moves;
    white_moves.reserve(MAX_BOARD_SIZE);
    black_moves.reserve(MAX_BOARD_SIZE);

    // Initialize the root node of the search tree.
    mcts_node* root = new mcts_node(game->own_color);
    init_mcts_node(root, &game_copy);
    root->result = check_win(&game_copy, false);

    time_t start = time(nullptr);
    vector<mcts_node*> history(board_size + 1);

    mcts_node* current;
    mcts_node* next;
    TYPES result;
    float best_ucb_value;
    float current_ucb_value;

    // Actual MCTS loop
    while (time(nullptr) < start + TIMEOUT + 1) {
        game_copy = *game;  // Performs a copy
        int history_bound = -1;
        depth = -1;
        history[++history_bound] = current = root;
        to_move = game->own_color;

        white_moves.clear();
        black_moves.clear();
        white_moves_idx = 0;
        black_moves_idx = 0;
 
        // Selection Step
        while (current->result == EMPTY && current->checked == board_size) {
            depth++;
            best_ucb_value = INT_MIN;
            next = current->children[0];
 
            // Select next node according to UCB
            for (int i = 0; i < current->size; i++) {
                if (depth % 2)  // Minimizing player
                    current_ucb_value = 1 - current->children[i]->mc_value + C * sqrt(log(current->mc_count) / current->children[i]->mc_count);
                else  // Maximizing player
                    current_ucb_value = current->children[i]->mc_value + C * sqrt(log(current->mc_count) / current->children[i]->mc_count);

                if (best_ucb_value < current_ucb_value) {
                    best_ucb_value = current_ucb_value;
                    next = current->children[i];
                }
            }

            // Actually make the moves on the board

            if (to_move == BLACK)
                black_moves.push_back(next->move), black_moves_idx++;
            else
                white_moves.push_back(next->move), white_moves_idx++;

            to_move = play_move(&game_copy, next->move, to_move);
            history[++history_bound] = next;
            current = next; 
        }
 
        if (current->result != EMPTY) {
            result = current->result;
        } else {
            // Expansion Step
            if (current->children.size() == 0)
                init_mcts_node(current, &game_copy);

            // Make space for new move
            current->children.push_back(new mcts_node(to_move));

            // Play new move
            next = current->children[current->size++];
            next->move = current->checked++;

            if (to_move == BLACK)
                black_moves.push_back(next->move), black_moves_idx++;
            else
                white_moves.push_back(next->move), white_moves_idx++;

            to_move = play_move(&game_copy, next->move, to_move);

            history[++history_bound] = next;

            // Look for next available move for next time 'current' is seen
            while (next->checked < board_size && game_copy.board[next->checked] != EMPTY)
                next->checked++;

            // Simulation Step
            next->result = check_win(&game_copy, false);
            if (next->result == EMPTY) {
                result = rollout(&game_copy, to_move, &black_moves, &white_moves);
            } else {
                next->checked = board_size;  // No point in checking further moves
                result = next->result;
            }
        }

        // Backpropagation Step
        int largest_history_bound = history_bound;
        for (; history_bound >= 0; history_bound--) {
            history[history_bound]->mc_value *= history[history_bound]->mc_count;
            history[history_bound]->mc_value += (result == game->own_color);
            history[history_bound]->mc_value /= history[history_bound]->mc_count + 1;

            // Alter AMAF values
            if (history_bound != largest_history_bound) {
                if (history[history_bound]->player == BLACK) {
                    for (auto& i = black_moves_idx; i < black_moves.size(); i++) {
                        history[history_bound]->amaf_values[black_moves[i]] *= history[history_bound]->amaf_counts[black_moves[i]];
                        history[history_bound]->amaf_values[black_moves[i]] += (result == game->own_color);
                        history[history_bound]->amaf_values[black_moves[i]] /= ++history[history_bound]->amaf_counts[black_moves[i]];
                    }
                } else {
                    for (auto& i = white_moves_idx; i < white_moves.size(); i++) {
                        history[history_bound]->amaf_values[white_moves[i]] *= history[history_bound]->amaf_counts[white_moves[i]];
                        history[history_bound]->amaf_values[white_moves[i]] += (result == game->own_color);
                        history[history_bound]->amaf_values[white_moves[i]] /= ++history[history_bound]->amaf_counts[white_moves[i]];
                    }
                }
            }

            history[history_bound]->mc_count++;
        }

        best_depth = max(depth, best_depth);
    }

    // Figure out the best move seen
    float best_value = -1;
    for (int i = 0; i < root->size; i++ ) {
        cout << move_to_string(game, root->children[i]->move) << " " << root->children[i]->mc_value << " " << root->children[i]->mc_value + C * sqrt(log(root->mc_count) / root->children[i]->mc_count) << " " << root->mc_count << " " << root->children[i]->mc_count << endl;
        if (root->children[i]->mc_value > best_value) {
            best_value = root->children[i]->mc_value;
            best_move = root->children[i]->move;
        }
    }

    // Convert to human-readable form
    string move = move_to_string(game, best_move);
    cout << move << " " << best_depth << endl;
    sety(game, best_move);

    return;
}

/**
 * @brief Plays a random rollout given the current state of the board.
 * 
 * @param game The game object.
 * @return TYPES 
 */
TYPES rollout(Game* game, TYPES to_move, vector<int>* black_moves, vector<int>* white_moves) {
    vector<int> moves;
    moves.reserve(pow(game->board_size, 2));

    // Get all available moves
    for (int i = 0; i < pow(game->board_size, 2); i++) {
        if (game->board[i] == EMPTY)
            moves.push_back(i);
    }

    // Shuffle the moves
    shuffle(moves.begin(), moves.end(), rng);
 
    // Since a win is not changed by continued play,
    // We play out all moves and only search the board once.
    // This legit has like 4x speedup over always checking
    // if the game is over
    vector<int>::size_type i = 0;
    do {
        if (to_move == BLACK)
            black_moves->push_back(moves[i]);
        else
            white_moves->push_back(moves[i]);

        to_move = play_move(game, moves[i++], to_move);
    } while (i < moves.size());

    return check_win(game, false);
}

/**
 * @brief 
 * 
 * @param node 
 * @param game 
 * @param color 
 */
void init_mcts_node(mcts_node* node, Game* game) {
    int board_size = pow(game->board_size, 2) - game->move_cnt + 1;
    node->amaf_values = vector<int>(pow(game->board_size, 2), 0);
    node->amaf_counts = vector<int>(pow(game->board_size, 2), 0);
    node->children.reserve(board_size);

    // Look for first available open move
    while (node->checked < board_size && game->board[node->checked] != EMPTY)
        node->checked++;

    return;
}

/**
 * @brief 
 * 
 * @param game 
 * @param move 
 * @param to_move 
 * @return TYPES 
 */
TYPES play_move(Game* game, int move, TYPES to_move) {
    if (to_move == game->own_color)
        sety(game, move);
    else
        seto(game, move);
    
    game->last_move = move;
    game->move_cnt++;
    return (to_move == game->own_color) ? game->opp_color : game->own_color;
}

/**
 * @brief
 * 
 * @param game 
 * @param str 
 */
void seto(Game* game, int coord) {
    if (coord < 0)
        swap(game);
    else if (game->board[coord] == EMPTY) {
        game->board[coord] = game->opp_color;
    }

    return;
}

/**
 * @brief 
 * 
 * @param game 
 * @param str 
 */
void sety(Game* game, int coord) {
    if (coord < 0)
        swap(game);
    else if (game->board[coord] == EMPTY) {
        game->board[coord] = game->own_color;
    }

    return;
}

/**
 * @brief 
 * 
 * @param game 
 * @param str 
 * @return int 
 */
int str_to_move(Game* game, string str) {
    if (strcmp(str.c_str(), "swap") == 0)
        return -1;

    int column = atoi(str.substr(1, str.size()).c_str()) - 1;
    int row = str[0] - 'a';
    return row * game->board_size + column;
}

/**
 * @brief 
 * 
 * @param game 
 * @param move 
 * @return string 
 */
string move_to_string(Game* game, int move) {
    string result(1, move / game->board_size + 'a');
    result += to_string(move % game->board_size + 1);

    return result;
}
