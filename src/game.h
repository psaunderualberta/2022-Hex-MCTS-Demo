#ifndef __GAME_H__
#define __GAME_H__
#include "utility.h"
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
float check_win(Game* game, bool check_all_and_print);

void make_move(Game* game);
int play_move(Game* game, vector<int> move, int to_move);
int play_move(Game* game, string move, int to_move);
void undo_move(Game* game, vector<int> move);
void seto(Game* game, vector<int> coord);
void sety(Game* game, vector<int> coord);

vector<int> str_to_move(Game* game, string str);
string move_to_string(Game* game, vector<int> move);

#endif