#ifndef __UTILITY_H__
#define __UTILITY_H__
#include <vector>
#include <string>
#include <cmath>
#include <iostream>
#include <string>
#include <cstring>
#include <bitset>

const int MAX_BOARD_SIZE = 30;

using namespace std;

enum TYPES {
    WHITE,
    BLACK,
    BORDER,
    EMPTY
};

struct union_find_set {
    TYPES cell;
    union_find_set* parent;
    int index;
    int rank;
};

struct Game {
    int board_size;
    int move_cnt;
    bool swap_occur;
    TYPES own_color;
    TYPES opp_color;
    union_find_set board[MAX_BOARD_SIZE * MAX_BOARD_SIZE];
    vector<vector<int>> neighbours;
};

struct mcts_node {
    int visits = 0;
    int checked = 0;
    int size = 0;
    int actions = 0;
    float value = 0.0;
    int move;
    vector<mcts_node*> children;
    TYPES result = EMPTY;
    TYPES player;
};

vector<string> tokenize(string s, string delimiter);

#endif