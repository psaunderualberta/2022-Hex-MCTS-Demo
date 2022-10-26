#include "utility.h"

// https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
// Accessed January 18th, 2022
vector<string> tokenize(string s, string delimiter) {
        vector<string> tokens;
        long unsigned int pos;

        while ((pos = s.find(delimiter)) != string::npos) {
                tokens.push_back(s.substr(0, pos));
                s.erase(0, pos+delimiter.size());
        }

        tokens.push_back(s);
        return tokens;
}