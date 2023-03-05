#ifndef SRC_INPUTARGUMENTS_H
#define SRC_INPUTARGUMENTS_H

#include <string>
using namespace std;

// Smoothing parameter for estimating the probabilities
// int ALPHA = 2;

// Window size
// int K = 11;

class InputArguments {

    int alpha;
    int k;
    string filePath;

    public:

        InputArguments(string filePath, int alpha, int k);

        [[nodiscard]] int getAlpha() const;
        [[nodiscard]] int getK() const;
        [[nodiscard]] string getFilePath() const;

        // Don't add nodiscard - I may want to check the arguments and do nothing with the return value.
        bool checkArguments() const;

};


#endif //SRC_INPUTARGUMENTS_H
