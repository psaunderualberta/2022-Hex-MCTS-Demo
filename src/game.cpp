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
        game->board[i].cell = EMPTY;
        game->board[i].index = i;
        game->board[i].rank = 1;
        game->board[i].parent = &game->board[i];
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

        if (game->board[i].cell == WHITE)
            cout << "W ";
        else if (game->board[i].cell == BLACK)
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
    game->board[coord].cell = EMPTY;
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
    bitset<(int) pow(MAX_BOARD_SIZE, 2)> set1;
    bitset<(int) pow(MAX_BOARD_SIZE, 2)> set2;
    union_find_set* root;

    for (int i = 0; i < game->board_size; i++) {
        if (game->board[i * game->board_size].cell == WHITE) {
            root = find(&game->board[i * game->board_size]);
            set1.set(root->index);
        }

        if (game->board[(i + 1) * game->board_size - 1].cell == WHITE) {
            root = find(&game->board[(i + 1) * game->board_size - 1]);
            set2.set(root->index);
        }
    }

    for (int i = 0; i < pow(game->board_size, 2); i++) {
        if (set1.test(i) && set2.test(i)) {
            // cout << (game->own_color == WHITE ? 1 : -1) << endl;
            return WHITE;
        }

        set1.reset(i);
        set2.reset(i);
    }

    // Time to test BLACK
    for (int i = 0; i < game->board_size; i++) {
        if (game->board[i].cell == BLACK) {
            root = find(&game->board[i]);
            set1.set(root->index);
        }

        if (game->board[(int) pow(game->board_size, 2) - game->board_size + i].cell == BLACK) {
            root = find(&game->board[(int) pow(game->board_size, 2) - game->board_size + i]);
            set2.set(root->index);
        }
    }

    for (int i = 0; i < pow(game->board_size, 2); i++) {
        if (set1.test(i) && set2.test(i)) {
            // cout << (game->own_color == BLACK ? 1 : -1) << endl;
            return BLACK;
        }
    }

    // cout << "there3" << endl;
    // cout << 0 << endl;
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

    init_mcts_node(root, game, game->own_color);

    time_t start = time(nullptr);
    vector<mcts_node*> history(board_size + 1);

    mcts_node* current;
    mcts_node* next;
    TYPES result;
    float best_ucb_value;
    float current_ucb_value;
    int i = 0;

    // Actual MCTS loop
    while (time(nullptr) < start + TIMEOUT + 1) {
        i++;
        game_copy = *game;  // Performs a copy

        // Make the parents point to locations in the copied array.
        for (int i = 0; i < board_size; i++) {
            game_copy.board[i].parent = &game_copy.board[i];
        }

        int history_bound = -1;
        depth = -1;
        history[++history_bound] = current = root;

        // Selection
        while (current->result == EMPTY && current->checked == board_size) {
            depth++;
            best_ucb_value = INT_MIN;

            // Select next node according to UCB
            for (int i = 0; i < current->size; i++) {                
                current_ucb_value = current->children[i]->value + C * sqrt(log(current->actions) / current->children[i]->visits);

                if (best_ucb_value < current_ucb_value) {
                    best_ucb_value = current_ucb_value;
                    next = current->children[i];
                }
            }

            // TODO: Actually make the moves on the board
            to_move = play_move(&game_copy, next->move, to_move);
            history[++history_bound] = next;
            current = next; 
        }
 
        // Expansion
        if (current->checked != board_size) {
            if (current->children.size() == 0)
                init_mcts_node(current, &game_copy, history[history_bound-1]->player == WHITE ? BLACK : WHITE);

            current->children[current->size++]->move = current->checked++;

            // Look for next available move
            while (current->checked < board_size && game_copy.board[current->checked].cell != EMPTY)
                current->checked++;

            history[++history_bound] = current->children[current->size - 1];
            to_move = play_move(&game_copy, current->children[current->size - 1]->move, to_move);
        }

        // Simulation
        if (current->result == EMPTY) {
            current->result = check_win(&game_copy);
            if (current->result == EMPTY) {
                result = rollout(&game_copy, to_move);
            } else {
                result = current->result;
            }
        } else {
            // This node is terminal
            result = current->result;
        } 

        // Backpropagation
        for (; history_bound >= 0; history_bound--) {
            history[history_bound]->value *= history[history_bound]->visits;
            history[history_bound]->value += (result == history[history_bound]->player);
            history[history_bound]->value /= ++history[history_bound]->visits;
            history[history_bound]->actions++;
        }

        best_depth = max(depth, best_depth);
    }

    for (int i = 0; i < root->size; i++ ) {
        cout << move_to_string(game, root->children[i]->move) << " " << root->children[i]->value << " " << root->actions << " " << root->children[i]->visits << endl;
    }

    // Convert to human-readable form
    string move = move_to_string(game, best_move);
    cout << move << " " << best_depth << endl;
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
        if (game->board[i].cell == EMPTY)
            moves.push_back(i);
    }

    // Shuffle the moves
    random_device rd;
    default_random_engine rng(rd());
    shuffle(moves.begin(), moves.end(), rng);

    // Play until we reach a decisive conclusion
    TYPES result;
    while ((result = check_win(game)) == EMPTY) {
        to_move = play_move(game, moves.back(), to_move);
        moves.pop_back();
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
    node->children = vector<mcts_node*>(board_size);
 
    for (int i = 0; i < board_size; i++) {
        node->children[i] = new struct mcts_node;
        node->children[i]->visits = 0;
        node->children[i]->checked = 0;
        node->children[i]->size = 0;
        node->children[i]->actions = 0;
        node->children[i]->value = 0.0;
        node->children[i]->result = EMPTY;
        node->children[i]->player = (color == WHITE) ? BLACK : WHITE;
    }

    // Look for first available open move
    while (node->checked < board_size && game->board[node->checked].cell != EMPTY)
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
        sety(game, move_to_string(game, move));
    else
        seto(game, move_to_string(game, move));
    
    return (to_move == game->own_color) ? game->opp_color : game->own_color;
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
    else if (game->board[coord].cell == EMPTY) {
        game->board[coord].cell = game->opp_color;
        game->move_cnt++;

        for (auto& neighbour : game->neighbours[coord]) {
            if (game->board[neighbour].cell == game->opp_color) {
                union_sets(&game->board[neighbour], &game->board[coord]);
            }
        }

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
    else if (game->board[coord].cell == EMPTY) {
        game->board[coord].cell = game->own_color;
        game->move_cnt++;

        for (auto& neighbour : game->neighbours[coord]) {
            if (game->board[neighbour].cell == game->own_color)
                union_sets(&game->board[neighbour], &game->board[coord]);
        }
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
