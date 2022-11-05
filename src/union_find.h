#ifndef __UNION_FIND_H__
#define __UNION_FIND_H__
#include "utility.h"

using namespace std;

union_find_set* find(union_find_set* root);
void union_sets(union_find_set* x, union_find_set* y);

#endif