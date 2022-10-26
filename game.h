#ifndef __GAME_H__
#define __GAME_H__
#include "utility.h"
#include <iostream>
#include <string>
#include <cstring>

using namespace std;

struct Game {
    int board_size;
    TYPES own_color;
    TYPES opp_color;
    bool first_move;
    TYPES* board = NULL;
    int move_cnt;
};

void init_game(Game* game, int board_size);
void show_board(Game* game);
void swap(Game* game);
void unset(Game* game, int move);
bool check_win(Game* game);

int make_move(Game* game);
void seto(Game* game, string str);
void sety(Game* game, string str);

int str_to_move(Game* game, string str);
string move_to_string(Game* game, int move);

#endif