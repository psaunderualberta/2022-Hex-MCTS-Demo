#include "main.h"

using namespace std;

int main(int argc, char** argv) {
    Game* game = new struct Game;

    // User must put in color, otherwise don't know which
    // color to use.
    if (argc != 2) {
        cout << "Error: Must put in color!" << endl;
        exit(1);
    }
    
    string color = argv[1];
    if (color =="white") {
        game->own_color = WHITE;
        game->opp_color = BLACK;
    } else {
        game->own_color = BLACK;
        game->opp_color = WHITE;      
    }

    // Default board size of 11
    init_game(game, 11);

    // Main game loop
    vector<string> cmd;
    do {
        cmd = getCmd();
        if (cmd[0] == "init_board")
            init_game(game, atoi(cmd[1].c_str()));
        if (cmd[0] == "show_board")
            show_board(game);
        if (cmd[0] == "seto")
            seto(game, cmd[1]);
        if (cmd[0] == "sety")
            sety(game, cmd[1]);
        if (cmd[0] == "check_win")
            check_win(game);

    } while (cmd.size() && cmd[0] != "quit");

    return 0;
}

/**
 * @brief  Reads in a command from the shell prompt and 
 * Converts it to a format that is easier to
 * parse by the rest of the program.
 * 
 * @return vector<string> The command split along spaces
 */
vector<string> getCmd() {
        string input;
        string delimiter = " ";
        vector<string> tokens;

        // Read in command
        while (true) {
                getline(cin, input);
                if (!cin.fail())
                        break;

                cin.clear();
                cin.ignore();
        }

        // Split input using spaces to denote words
        tokens = tokenize(input, delimiter);

        return tokens;
}
