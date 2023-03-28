#include "generalUtils.h"

bool isNumber(char *s) {
    std::string arrayAsString(s);
    return std::regex_match(arrayAsString, std::regex("[0-9]*[\\.]?[0-9]+"));
}

double calculateHitProbability(const unsigned int Nh, const unsigned int Nf, const double alpha) {
    return (double) (Nh + alpha) / (Nh + Nf + 2*alpha);
}

std::string convertCharArrayToString(char* a, int size) {
    int i;
    std::string s;
    for (i = 0; i < size; i++)
        s += a[i];
    return s;
}

std::string convertCharVectorToString(std::vector<char> vector) {
    int i;
    std::string s;
    for (i = 0; i < vector.size(); i++)
        s += vector[i];
    return s;
}

bool sort_by_value(const std::pair<char, double>& a, const std::pair<char, double>& b) {
    return a.second > b.second;
}



