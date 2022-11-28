#ifndef __UTILITY_H__
#define __UTILITY_H__
#include <vector>
#include <string>
#include <cmath>

const int MAX_BOARD_SIZE = 15;
const int MAX_Y_BOARD_SIZE = MAX_BOARD_SIZE * 2 - 1;

using namespace std;

const int WHITE = 1;
const int BLACK = -1;
const int EMPTY = 0;

struct Game {
    int board_size;
    int y_board_size;
    int move_cnt;
    int own_color;
    int opp_color;
    float eval[MAX_Y_BOARD_SIZE][MAX_Y_BOARD_SIZE][MAX_Y_BOARD_SIZE];
    int board[MAX_Y_BOARD_SIZE][MAX_Y_BOARD_SIZE];
    bool swap_occur;
};

vector<string> tokenize(string s, string delimiter);

#endif