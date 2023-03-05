//
// Created by hugo on 05-03-2023.
//

#ifndef SRC_UTILS_H
#define SRC_UTILS_H

#include <cstring>
#include <string>
#include <regex>

bool isNumber(char *s) {
    std::string arrayAsString(s);
    return std::regex_match(arrayAsString, std::regex("[0-9]*[\\.]?[0-9]+"));
}

double calculateHitProbability(int Nh, int Nf, double alpha) {
    return (double) (Nh + alpha) / (Nh + Nf + 2*alpha);
}

#endif //SRC_UTILS_H
