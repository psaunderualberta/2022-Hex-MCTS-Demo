#ifndef __GAME_H__
#define __GAME_H__
#include "utility.h"
#include <iostream>
#include <string>
#include <cstring>
#include <random>
#include <algorithm>
#include <climits>

using namespace std;

const int TIMEOUT = 5;
const int C = 2;

void init_game(Game* game, int board_size);
void init_neighbours(Game* game);
void show_board(Game* game);
void swap(Game* game);
void unset(Game* game, int move);
TYPES check_win(Game* game);

void make_move(Game* game);
TYPES play_move(Game* game, int move, TYPES to_move);
void seto(Game* game, string str);
void sety(Game* game, string str);

int str_to_move(Game* game, string str);
string move_to_string(Game* game, int move);

int MCTS(Game* game);
void init_mcts_node(mcts_node* node, Game* game, TYPES color);
TYPES rollout(Game* game, TYPES to_move);

#endif