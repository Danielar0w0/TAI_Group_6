#ifndef SRC_INPUT_H
#define SRC_INPUT_H

#include <string>
using namespace std;

// Smoothing parameter for estimating the probabilities
// int ALPHA = 2;

// Window size
// int K = 11;

class Input {
    private:
        int alpha;
        int k;
        string filePath;

    public:

        explicit Input(string filePath, int alpha, int k) {
            this->alpha = alpha;
            this->k = k;
            this->filePath = filePath;
        }

        int getAlpha() const {
            return this->alpha;
        }

        int getK() const {
            return this->k;
        }

        string getPath() {
            return this->filePath;
        }
};


#endif //SRC_INPUT_H
