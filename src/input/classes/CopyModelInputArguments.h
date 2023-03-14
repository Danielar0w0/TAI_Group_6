#ifndef SRC_COPYMODELINPUTARGUMENTS_H
#define SRC_COPYMODELINPUTARGUMENTS_H

#include <string>
using namespace std;

// Smoothing parameter for estimating the probabilities
// int ALPHA = 2;

// Window size
// int K = 11;

class CopyModelInputArguments {

    double alpha;
    int k;
    string filePath;

    public:

        CopyModelInputArguments(string filePath, double alpha, int k);

        [[nodiscard]] double getAlpha() const;
        [[nodiscard]] int getK() const;
        [[nodiscard]] string getFilePath() const;

        // Don't add nodiscard - I may want to check the arguments and do nothing with the return value.
        bool checkArguments() const;

        static void printUsage();

};


#endif //SRC_COPYMODELINPUTARGUMENTS_H