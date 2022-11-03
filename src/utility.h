#ifndef __UTILITY_H__
#define __UTILITY_H__
#include <vector>
#include <string>
#include <cmath>

const int MAX_BOARD_SIZE = 30;

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
    TYPES board[MAX_BOARD_SIZE * MAX_BOARD_SIZE];
    vector<vector<int>> neighbours;
};

vector<string> tokenize(string s, string delimiter);

#endif