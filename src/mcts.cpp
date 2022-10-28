#include "mcts.h" 

/**
 * @brief 
 * 
 * @param game 
 * @return int The move to make
 */
int MCTS(Game* game) {
    int best_move = 0;
    int depth = 0;
    int board_size = pow(game->board_size, 2);

    // Initialize the root node of the search tree.
    mcts_node* root = new struct mcts_node;
    // root->player = game->own_color;

    root->visits = 0;
    root->checked = 0;
    root->size = 0;
    root->actions = 0;
    root->value = 0.0;
    root->result = EMPTY;
    root->player = game->own_color;

    init_mcts_node(root, game, game->own_color);

    time_t start = time(nullptr);
    vector<mcts_node*> history(board_size + 1);

    mcts_node* current;
    mcts_node* next;
    TYPES result;
    int best_ucb_value;

    // Actual MCTS loop
    while (time(nullptr) < start + TIMEOUT + 1) {
        int history_bound = -1;
        current = root;
        history[++history_bound] = root;

        // cout << current->result << " " << current->checked << endl;

        // Selection
        while (current->result == EMPTY && current->checked == board_size) {
            depth++;
            // cout << depth << endl;
            best_ucb_value = INT_MIN;
            // Select next node according to UCB
            for (int i = 0; i < current->size; i++) {
                // cout << current->children[i].actions << " " << current->children[i].viqsits << endl;
                if (best_ucb_value < current->children[i].value + C * sqrt(log(current->children[i].actions) / current->children[i].visits)) {
                    best_ucb_value = current->children[i].value + C * sqrt(log(current->children[i].actions) / current->children[i].visits);
                    next = &current->children[i];
                }
            }

            // TODO: Actually make the moves on the board
            history[++history_bound] = next;
            current = next;
        }

        if (current->result == EMPTY) {
            // Expansion
            if (current->children == NULL)
                init_mcts_node(current, game, history[history_bound-1]->player == WHITE ? BLACK : WHITE);

            current->children[current->size++].move = current->checked++;

            // Look for next available move
            while (current->checked < board_size && game->board[current->checked] != EMPTY)
                current->checked++;

            // TODO: Play item on game->board[current->size - 1]
            history[++history_bound] = &current->children[current->size - 1];
            // Simulation
            result = rollout(game);
        } else {

            // This node is terminal
            result = current->result;
        }

        // Backpropagation
        for (; history_bound; history_bound--) {
            history[history_bound]->value *= history[history_bound]->visits;
            history[history_bound]->value += 2 * (result == history[history_bound]->player) - 1;
            history[history_bound]->value /= ++history[history_bound]->visits;
            history[history_bound]->actions++;
        }
    }

    // TODO: Free all nodes (needs to be recursive :( )

    free(root->children);
    cout << depth << endl;
    return best_move;
}

void init_mcts_node(mcts_node* node, Game* game, TYPES color) {
    int board_size = pow(game->board_size, 2);
    node->children = (mcts_node*) malloc(board_size * sizeof(mcts_node));

    for (int i = 0; i < board_size; i++) {
        node->children[i].visits = 0;
        node->children[i].checked = 0;
        node->children[i].size = 0;
        node->children[i].actions = 0;
        node->children[i].value = 0.0;
        node->children[i].result = EMPTY;
        node->children[i].player = (color == WHITE) ? BLACK : WHITE;
    }

    // Look for next available spot
    while (node->checked < board_size && game->board[node->checked] != EMPTY)
        node->checked++;

    return;
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