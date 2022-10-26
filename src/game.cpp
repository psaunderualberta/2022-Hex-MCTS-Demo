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
    game->size_of_board_array = game->board_size * (game->board_size + 1) + 1;

    if (game->board != NULL)
        free(game->board);
    game->board = (TYPES*) malloc(game->size_of_board_array * sizeof(TYPES));

    for (int i = 0; i < game->size_of_board_array; i++) {
        if (i % game->board_size == 0)
            game->board[i] = BORDER;
        else
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
    vector<int> offsets = { -1, 1, -game->board_size, game->board_size, -game->board_size+1, game->board_size-1 };
    int pos;

    for (int i = 0; i < game->size_of_board_array; i++) {
        game->neighbours.push_back(vector<int>());
        if (game->board[i] == BORDER)
            continue;

        for (auto& offset : offsets) {
            pos = i + offset;
            if (0 <= pos && pos < game->size_of_board_array && game->board[pos] != BORDER)
                game->neighbours.back().push_back(pos);
        }
    }
}

// void show_board(Game* game) {

// }

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
bool check_win(Game* game, TYPES color) {
    vector<bool> seen(game->size_of_board_array, false);
    string coord;
    int move;

    for (int i = 0; i < game->board_size; i++) {
        // TODO: Establish which direction to look
        coord = to_string('a' + i) + "0";
        move = str_to_move(game, coord);
        if (game->board[move] == color && !seen[move]) {
            vector<int> bfs = { move };
            seen[move] = true;
            vector<int>::size_type pos = 0;

            while (pos < bfs.size()) {
                move = bfs[i];
                for (auto& next_move : game->neighbours[move]) {
                    if (next_move == color && !seen[move]) {

                        // win condition
                        coord = move_to_string(game, next_move);
                        if (coord[0] == 'a' + game->board_size)
                            return true;

                        bfs.push_back(next_move);
                        seen[next_move] = true;
                    }
                }
            }
        }
    }

    return false;
}

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
    string letter = to_string(move / game->board_size + 'a'); 
    string number = to_string(move % game->board_size + 1);

    return letter + number;
}
