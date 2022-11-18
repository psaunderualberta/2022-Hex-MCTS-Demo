# Heuristic Ideas

## 3x3 simplification
Preprocess all possible 3x3 boards and their outcomes (minimax or some shit). Then sweep over the board calculating the 3x3 results for each individual board,
and then search in this reduced space for a heuristic evaluation.

## Expanding possibility of opening
Only consider the central square of size 'n' when playing move 'n', or some relationship between.