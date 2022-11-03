#ifndef __GAME_H__
#define __GAME_H__
#include "utility.h"
#include "mcts.h"
#include <iostream>
#include <string>
#include <cstring>
#include <random>
#include <algorithm>

using namespace std;

void init_game(Game* game, int board_size);
void init_neighbours(Game* game);
void show_board(Game* game);
void swap(Game* game);
void unset(Game* game, int move);
TYPES check_win(Game* game);

void make_move(Game* game);
void seto(Game* game, string str);
void sety(Game* game, string str);

int str_to_move(Game* game, string str);
string move_to_string(Game* game, int move);

#endif