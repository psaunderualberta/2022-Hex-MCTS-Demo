#ifndef __MCTS_H__
#define __MCTS_H__
#include "utility.h"

const int TIMEOUT = 10;

struct mcts_node {
    int visits;
    int* actions;
    int* values;
};

int MCTS(Game* game);
TYPES rollout(Game* game);

#endif