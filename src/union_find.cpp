#include "union_find.h"

union_find_set* find(union_find_set* root) {
    if (root->parent != root) {
        root->parent = find(root->parent);
        return root->parent;
    }

    return root;
}

void union_sets(union_find_set* x, union_find_set* y) {
    x = find(x);
    y = find(y);

    // Same cell, don't need to do anything
    if (x == y)
        return;
    
    if (x->rank < y->rank) {
        union_find_set* t = x;
        x = y;
        y = t;
    }

    y->parent = x;
    if (y->rank == x->rank) {
        x->rank++;
    }

    return;
}