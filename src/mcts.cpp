#include "mcts.h" 

/**
 * @brief 
 * 
 * @param game 
 * @return int The move to make
 */
int MCTS(Game* game) {
    int best_move = 0;

    int number_of_empties_at_root = 0;
    for (int i = 0; i < pow(game->board_size, 2); i++) {
        number_of_empties_at_root += (game->board[i] == EMPTY);
    }

    // Initialize the root node of the search tree.
    mcts_node* root = new struct mcts_node;
    root->children = (mcts_node*) malloc((number_of_empties_at_root - 1) * sizeof(mcts_node));
    root->values = (int*) malloc((number_of_empties_at_root - 1) * sizeof(int));
    root->actions = (int*) malloc((number_of_empties_at_root - 1) * sizeof(int));

    time_t start = time(nullptr);
    vector<mcts_node*> history(pow(game->board_size, 2) + 1);
    int history_bound = 0;

    while (time(nullptr) < start + TIMEOUT + 1) {
        // Selection
        // TODO: HOW THE FUCK DO I PICK A VALID MOVE IN O(1) TIME?

        // Expansion

        // Simulation

        // Backprop
        TYPES result = rollout(game);
        for (; history_bound; history_bound--) {
            
        }
    }

    // TODO: Free all nodes (needs to be recursive :( )

    free(root->children);
    free(root->values);
    free(root->actions);

    return best_move;
}

/**
 * @brief 
 * 
 * @param game 
 * @return TYPES 
 */
TYPES rollout(Game* game) {


    return game->own_color;
}