#ifndef __MCTS_H__
#define __MCTS_H__
#include "utility.h"
#include <cstring>
#include <climits>
#include <iostream>

const int TIMEOUT = 85;
const int C = 2;

struct mcts_node {
    int visits = 0;
    int checked = 0;
    int size = 0;
    int actions = 0;
    float value = 0.0;
    int move;
    TYPES result = EMPTY;
    TYPES player;
    vector<mcts_node*> children;

    // Initializer
    mcts_node(TYPES color) : visits(0), checked(0), size(0), actions(0), value(0.0), move(-1), result(EMPTY), player(color == WHITE ? BLACK : WHITE) {};
};

int MCTS(Game* game);
void init_mcts_node(mcts_node* node, Game* game);
TYPES rollout(Game* game, TYPES to_move);

#endif