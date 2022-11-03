#include "game.h"

/**
 * @brief 
 * 
 * @param game 
 * @param board_size 
 */
void init_game(Game* game, int board_size) {

    // Create board size
    game->board_size = board_size;

    if (board_size > MAX_BOARD_SIZE) {
        cout << "ERROR: Board size must be less than " << MAX_BOARD_SIZE << endl;
        exit(1);
    }

    for (int i = 0; i < game->board_size * game->board_size; i++) {
        game->board[i] = EMPTY;
    }

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
TYPES check_win(Game* game) {
    vector<bool> seen(game->board_size * game->board_size, false);
    string coord;
    TYPES current_color;
    int minPos, maxPos, minModPos, maxModPos;
    vector<int>::size_type pos;

    for (int move = 0; move < game->board_size * game->board_size; move++) {
        // TODO: Establish which direction to look
        if (game->board[move] != EMPTY && !seen[move]) {
            current_color = game->board[move];
            vector<int> bfs = { move };
            seen[move] = true;
            pos = 0;

            // Get the best locations of the current path
            minPos = move, maxPos = move;
            minModPos = move % game->board_size, maxModPos = move % game->board_size;

            while (pos < bfs.size()) {
                move = bfs[pos];
                for (auto& next_move : game->neighbours[move]) {
                    if (game->board[next_move] != EMPTY && !seen[next_move]) {

                        minPos = min(minPos, next_move);
                        minModPos = min(minModPos, next_move % game->board_size);
                        maxPos = max(maxPos, next_move);
                        maxModPos = max(maxModPos, next_move % game->board_size);

                        // Win conditions
                        // White touches left & right sides
                        // Black touches top & bottom
                        if (current_color == WHITE && minModPos == 0 && maxModPos == game->board_size - 1) {
                            // cout << (game->own_color == WHITE ? 1 : -1) << endl;
                            return WHITE;
                        } else if (current_color == BLACK && minPos < game->board_size && game->board_size * (game->board_size - 1) <= maxPos) {
                            // cout << (game->own_color == BLACK ? 1 : -1) << endl;
                            return BLACK;
                        }

                        bfs.push_back(next_move);
                        seen[next_move] = true;
                    }
                }

                pos++;
            }
        }
    }

    // No winner yet
    // cout << "0" << endl;
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
    int board_size = pow(game->board_size, 2);
    TYPES to_move = game->own_color;

    Game game_copy = *game;
    // Initialize the root node of the search tree.
    mcts_node* root = new struct mcts_node;
    root->player = game->own_color;

    root->visits = 0;
    root->checked = 0;
    root->size = 0;
    root->actions = 0;
    root->value = 0.0;
    root->result = EMPTY;
    root->player = game->own_color;
    root->children = NULL;

    init_mcts_node(root, game, game->own_color);

    time_t start = time(nullptr);
    vector<mcts_node*> history(board_size + 1);

    mcts_node* current;
    mcts_node* next;
    TYPES result;
    int best_ucb_value;

    // Actual MCTS loop
    while (time(nullptr) < start + TIMEOUT + 1) {
        game_copy = *game;
        int history_bound = -1;
        current = root;
        history[++history_bound] = root;

        // cout << current->result << " " << current->checked << endl;

        // Selection
        while (current->result == EMPTY && current->checked == board_size) {
            depth++;
            // cout << depth << endl;
            best_ucb_value = INT_MIN;
            // Select next node according to UCB
            for (int i = 0; i < current->size; i++) {
                // cout << current->children[i].actions << " " << current->children[i].viqsits << endl;
                if (best_ucb_value < current->children[i].value + C * sqrt(log(current->children[i].actions) / current->children[i].visits)) {
                    best_ucb_value = current->children[i].value + C * sqrt(log(current->children[i].actions) / current->children[i].visits);
                    next = &current->children[i];
                }
            }

            // TODO: Actually make the moves on the board
            game_copy.board[next->move] = to_move;
            to_move = (to_move == BLACK) ? WHITE : BLACK;
            history[++history_bound] = next;
            current = next;
        }

        if (current->result == EMPTY) {
            // Expansion
            if (current->children == NULL)
                init_mcts_node(current, game, history[history_bound-1]->player == WHITE ? BLACK : WHITE);

            current->children[current->size++].move = current->checked++;

            // Look for next available move
            while (current->checked < board_size && game->board[current->checked] != EMPTY)
                current->checked++;

            // TODO: Play item on game->board[current->size - 1]
            history[++history_bound] = &current->children[current->size - 1];
            game_copy.board[current->children[current->size - 1].move] = to_move;
            to_move = (to_move == BLACK) ? WHITE : BLACK;

            // Simulation
            result = rollout(game, to_move);
        } else {
            // This node is terminal
            result = current->result;
        }

        // Backpropagation
        for (; history_bound >= 0; history_bound--) {
            history[history_bound]->value *= history[history_bound]->visits;
            history[history_bound]->value += (result == history[history_bound]->player) ? 1 : -1;
            history[history_bound]->value /= ++history[history_bound]->visits;
            if (history_bound == 0)
                cout << history[history_bound]->value << endl;
            history[history_bound]->actions++;
        }
    }

    // TODO: Free all nodes (needs to be recursive :( )


    for (int i = 0; i < root->size; i++ ) {
        cout << move_to_string(game, root->children[i].move) << " " << root->children[i].value << " " << root->children[i].actions << " " << root->children[i].visits << endl;
    }
 
    free(root->children);

    // Convert to human-readable form
    string move = move_to_string(game, best_move);
    cout << move << endl;
    sety(game, move);

    return;
}

/**
 * @brief Plays a random rollout given the current state of the board.
 * 
 * @param game The game object.
 * @return TYPES 
 */
TYPES rollout(Game* game, TYPES to_move) {
    vector<int> moves;

    // Get all available moves
    for (int i = 0; i < pow(game->board_size, 2); i++) {
        if (game->board[i] == EMPTY)
            moves.push_back(i);
    }

    // Shuffle the moves
    random_device rd;
    default_random_engine rng(rd());
    shuffle(moves.begin(), moves.end(), rng);

    // Play until we reach a decisive conclusion
    TYPES result;
    while ((result = check_win(game)) == EMPTY) {
        game->board[moves.back()] = to_move;
        moves.pop_back();
        to_move = (to_move == BLACK) ? WHITE : BLACK;
    }

    return result;
}

/**
 * @brief 
 * 
 * @param node 
 * @param game 
 * @param color 
 */
void init_mcts_node(mcts_node* node, Game* game, TYPES color) {
    int board_size = pow(game->board_size, 2);
    node->children = (mcts_node*) malloc(board_size * sizeof(mcts_node));

    for (int i = 0; i < board_size; i++) {
        node->children[i].visits = 0;
        node->children[i].checked = 0;
        node->children[i].size = 0;
        node->children[i].actions = 0;
        node->children[i].value = 0.0;
        node->children[i].result = EMPTY;
        node->children[i].player = (color == WHITE) ? BLACK : WHITE;
    }

    // Look for first available open move
    while (node->checked < board_size && game->board[node->checked] != EMPTY)
        node->checked++;

    return;
}

/**
 * @brief
 * 
 * @param game 
 * @param str 
 */
void seto(Game* game, string str) {
    int coord = str_to_move(game, str);

    if (coord < 0)
        swap(game);
    else if (game->board[coord] == EMPTY) {
        game->board[coord] = game->opp_color;
        game->move_cnt++;
    }

    return;
}

/**
 * @brief 
 * 
 * @param game 
 * @param str 
 */
void sety(Game* game, string str) {
    int coord = str_to_move(game, str);

    if (coord < 0)
        swap(game);
    else if (game->board[coord] == EMPTY) {
        game->board[coord] = game->own_color;
        game->move_cnt++;
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
