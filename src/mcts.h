#ifndef __MCTS_H__
#define __MCTS_H__
#include "utility.h"

const int TIMEOUT = 1;

struct mcts_node {
    int visits = 0;
    int* actions;
    int* values;
    mcts_node* children;
    TYPES player;
};

int MCTS(Game* game);
TYPES rollout(Game* game);

#endif