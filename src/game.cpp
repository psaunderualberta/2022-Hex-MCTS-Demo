#include "game.h"
// https://www.cs.cornell.edu/~adith/docs/y_hex.pdf

/**
 * @brief 
 * 
 * @param game 
 * @param board_size 
 */
void init_game(Game* game, int board_size) {

    // Create board size
    game->board_size = board_size;
    game->y_board_size = 2 * board_size - 1;

    if (board_size > MAX_BOARD_SIZE) {
        cout << "ERROR: Board size must be less than " << MAX_BOARD_SIZE << endl;
        exit(1);
    }

    for (int i = 0; i < game->y_board_size; i++) {
        for (int j = 0; j < game->y_board_size; j++)
            game->board[i][j] = EMPTY;
    }

    // Initialize triangle of white hexagons
    for (int i = 0; i < board_size-1; i++) {
        for (int j = 0; j < board_size-i-1; j++) {
            game->board[i][j] = BLACK;
        }
    }

    // Initialize triangle of black hexagons
    for (int i = board_size; i < 2 * board_size; i++) {
        for (int j = 0; j < 2 * board_size - i - 1; j++) {
            game->board[i][j] = WHITE;
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
    for (int start_j = 0; start_j < game->board_size; start_j++) {
        int j = start_j;
        for (int i = 0; i < start_j; i++)
            cout << " ";
        for (int i = game->board_size-1; i >= 0; i--, j++) {
            if (game->board[i][j] == WHITE)
                cout << "W ";
            else if (game->board[i][j] == BLACK)
                cout << "B ";
            else
                cout << ". ";
        }
        cout << endl;
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

    int t = game->own_color;
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
    vector<int> coord = str_to_move(game, move);
    game->board[coord[0]][coord[1]] = EMPTY;
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
float check_win(Game* game, bool check_all_and_print) {

    if (check_all_and_print)
        cout << WHITE << endl;
    
    for (int i = 0; i < game->y_board_size; i++) {
        for (int j = 0; j < game->y_board_size - i; j++) {
            game->eval[i][j][game->y_board_size-1] = game->board[i][j];  // Set initial evaluation
        }
    }

    for (int k = game->y_board_size - 2; k >= 0; k--) {
        for (int i = 0; i < k; i++) {
            for (int j = 0; j < k - i; j++) {
                game->eval[i][j][k] = 0.5 * (
                    game->eval[i][j][k + 1] + game->eval[i+1][j][k+1] + game->eval[i][j+1][k+1]
                    - game->eval[i][j][k + 1] * game->eval[i+1][j][k+1] * game->eval[i][j+1][k+1]
                );
            }
        }
    }

    return game->own_color;
}

float IDDFS(Game* game, vector<vector<int>> moves, int depth, int to_move) {
    if (!depth)
        return check_win(game, false);

    float heuristic = 0;
    int count = 0;
    for (auto& move : moves) {
        if (game->board[move[0]][move[1]] == EMPTY) {
            play_move(game, move, to_move);
            heuristic *= count;
            heuristic += IDDFS(game, moves, depth - 1, to_move == WHITE ? BLACK : WHITE);
            heuristic /= ++count;
            undo_move(game, move);
        }
    }

    return heuristic;
}

/**
 * @brief Performs the actual MCTS search
 * 
 * @param game 
 */
void make_move(Game* game) {
    Game game_copy;

    game_copy = *game;  // makes a copy

    vector<vector<int>> moves;
    for (char c = 'a'; c < 'a' + game->board_size; c++) {
        for (int j = 1; j <= game->board_size; j++) {
            string s1(1, c);
            moves.push_back(str_to_move(game, s1 + to_string(j)));
        }
    }

    cout << IDDFS(game, moves, 3, WHITE) << endl;

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
int play_move(Game* game, vector<int> move, int to_move) {
    if (to_move == game->own_color)
        sety(game, move);
    else
        seto(game, move);
    
    game->move_cnt++;
    return (to_move == game->own_color) ? game->opp_color : game->own_color;
}

/**
 * @brief 
 * 
 * @param game 
 * @param move 
 */
void undo_move(Game* game, vector<int> move) {
    game->board[move[0]][move[1]] = EMPTY;
    game->move_cnt++;

    return;
}

/**
 * @brief 
 * 
 * @param game 
 * @param move 
 * @param to_move 
 * @return int 
 */
int play_move(Game* game, string move, int to_move) {
    vector<int> moves = str_to_move(game, move);
    return play_move(game, moves, to_move);
}

/**
 * @brief
 * 
 * @param game 
 * @param str 
 */
void seto(Game* game, vector<int> coord) {
    if (coord[0] < 0)
        swap(game);
    else if (game->board[coord[0]][coord[1]] == EMPTY) {
        game->board[coord[0]][coord[1]] = game->opp_color;
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
void sety(Game* game, vector<int> coord) {
    if (coord[0] < 0)
        swap(game);
    else if (game->board[coord[0]][coord[1]] == EMPTY) {
        game->board[coord[0]][coord[1]] = game->own_color;
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
vector<int> str_to_move(Game* game, string str) {
    if (strcmp(str.c_str(), "swap") == 0)
        return {-1, -1};


    int column = game->board_size - 1 + (str[0] - 'a');
    int row = 0;
    int steps = atoi(str.substr(1, str.size()).c_str()) - 1;
    row += game->board_size - steps - 1;
    column -= game->board_size - steps - 1;
    return {row, column};
}

/**
 * @brief 
 * 
 * @param game 
 * @param move 
 * @return string 
 */
string move_to_string(Game* game, vector<int> move) {
    // TODO: Fix
    string result(1, game->board_size - move[0] + 'a');
    result += to_string(move[1] - (game->board_size - move[0]) + 1);

    return result;
}
