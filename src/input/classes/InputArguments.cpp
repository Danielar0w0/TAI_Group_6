#include "InputArguments.h"

#include <iostream>

InputArguments::InputArguments(string filePath, double alpha, int k) {
    this->alpha = alpha;
    this->k = k;
    this->filePath = std::move(filePath);
}

double InputArguments::getAlpha() const {
    return this->alpha;
}

int InputArguments::getK() const {
    return this->k;
}

string InputArguments::getFilePath() const {
    return this->filePath;
}

bool InputArguments::checkArguments() const {

    if (this->alpha <= 0 || this->alpha > 5) {
        std::cerr << "[!!!] Alpha should be a value in the interval ]0, 5]." << endl;
        return false;
    }

    if (this->k <= 0 || this->k > 100) {
        std::cerr << "[!!!] K should be a value in the interval ]0, 100]." << endl;
        return false;
    }

    if (this->filePath.empty()) {
        std::cerr << "[!!!] The target file was not provided." << endl;
        return false;
    }

    return true;

}
