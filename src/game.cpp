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

    if (game->board != NULL)
        free(game->board);

    game->board = (TYPES*) malloc(game->board_size * game->board_size * sizeof(TYPES));

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
bool check_win(Game* game) {
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

                        // win conditions
                        // White touches left & right sides
                        // Black touches top & bottom
                        if (current_color == WHITE && minModPos == 0 && maxModPos == game->board_size - 1) {
                            cout << (game->own_color == WHITE ? 1 : -1) << endl;
                            return true;
                        } else if (current_color == BLACK && minPos < game->board_size && game->board_size * (game->board_size - 1) <= maxPos) {
                            cout << (game->own_color == BLACK ? 1 : -1) << endl;
                            return true;
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
    cout << "0" << endl;
    return false;
}

// /**
//  * @brief Performs the actual MCTS search
//  * 
//  * @param game 
//  * @return int 
//  */
// int make_move(Game* game) {

// }

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
