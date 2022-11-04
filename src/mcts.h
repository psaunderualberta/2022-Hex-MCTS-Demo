#ifndef __MCTS_H__
#define __MCTS_H__
#include "utility.h"
#include <cstring>
#include <climits>
#include <iostream>

const int TIMEOUT = 1;
const int C = 2;

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

int MCTS(Game* game);
void init_mcts_node(mcts_node* node, Game* game, TYPES color);
TYPES rollout(Game* game, TYPES to_move);

#endif