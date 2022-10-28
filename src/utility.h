#ifndef __UTILITY_H__
#define __UTILITY_H__
#include "utility.h"
#include <vector>
#include <string>
#include <cmath>

using namespace std;

enum TYPES {
    WHITE,
    BLACK,
    BORDER,
    EMPTY
};

struct Game {
    int board_size;
    int move_cnt;
    bool swap_occur;
    TYPES own_color;
    TYPES opp_color;
    TYPES* board = NULL;
    vector<vector<int>> neighbours;
};

vector<string> tokenize(string s, string delimiter);

#endif