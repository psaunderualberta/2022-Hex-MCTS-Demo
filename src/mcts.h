#ifndef __MCTS_H__
#define __MCTS_H__
#include "utility.h"
#include <cstring>
#include <climits>
#include <iostream>

const int TIMEOUT = 85;
const int C = 2;

struct mcts_node {
    int checked = 0;
    int size = 0;
    int mc_count = 0;
    float mc_value = 0.0;
    int move;
    TYPES result = EMPTY;
    TYPES player;
    vector<int> amaf_values;
    vector<int> amaf_counts;
    vector<mcts_node*> children;

    // Initializer
    mcts_node(TYPES color) : checked(0), size(0), mc_count(0), mc_value(0.0),
                             move(-1), result(EMPTY), player(color == WHITE ? BLACK : WHITE) {};
};

int MCTS(Game* game);
void init_mcts_node(mcts_node* node, Game* game);
TYPES rollout(Game* game, TYPES to_move, vector<int>* black_moves, vector<int>* white_moves);

#endif