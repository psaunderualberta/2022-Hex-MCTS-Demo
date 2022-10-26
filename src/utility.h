#ifndef __UTILITY_H__
#define __UTILITY_H__
#include "utility.h"
#include <vector>
#include <string>

using namespace std;

struct mcts_node {

};

enum TYPES {
    WHITE,
    BLACK,
    BORDER,
    EMPTY
};

vector<string> tokenize(string s, string delimiter);

#endif