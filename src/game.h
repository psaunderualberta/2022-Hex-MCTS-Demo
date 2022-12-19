#ifndef __GAME_H__
#define __GAME_H__
#include "utility.h"
#include "mcts.h"
#include <iostream>
#include <string>
#include <cstring>
#include <random>
#include <algorithm>
#include <bitset>

using namespace std;

void init_game(Game* game, int board_size);
void init_neighbours(Game* game);
void show_board(Game* game);
void swap(Game* game);
void unset(Game* game, int move);
TYPES check_win(Game* game, bool check_all_and_print);

void make_move(Game* game);
TYPES play_move(Game* game, int move, TYPES to_move);
void seto(Game* game, int coord);
void sety(Game* game, int coord);

int str_to_move(Game* game, string str);
string move_to_string(Game* game, int move);

#endif