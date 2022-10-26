#include "game.h"

void init_game(Game* game, int board_size) {

    // Create board size
    game->board_size = board_size;
    int size_of_board = game->board_size * (game->board_size + 1) + 1;

    if (game->board != NULL)
        free(game->board);
    game->board = (TYPES*) malloc(size_of_board * sizeof(TYPES));

    for (int i = 0; i < size_of_board; i++) {
        if (i % game->board_size == 0)
            game->board[i] = BORDER;
        else
            game->board[i] = EMPTY;
    }

    return;
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

// bool check_win(Game* game) {

// }

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
