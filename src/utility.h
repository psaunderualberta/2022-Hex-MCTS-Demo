#ifndef __UTILITY_H__
#define __UTILITY_H__
#include "utility.h"
#include <vector>
#include <string>

using namespace std;

enum TYPES {
    WHITE,
    BLACK,
    BORDER,
    EMPTY
};

struct Game {
    int board_size;
    TYPES own_color;
    TYPES opp_color;
    bool first_move;
    TYPES* board = NULL;
    int move_cnt;
    vector<vector<int>> neighbours;
};

vector<string> tokenize(string s, string delimiter);

#endif