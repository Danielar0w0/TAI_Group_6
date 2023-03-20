#ifndef SRC_GENERALUTILS_H
#define SRC_GENERALUTILS_H

#include <cstring>
#include <string>
#include <regex>

bool isNumber(char *s);
double calculateHitProbability(unsigned int Nh, unsigned int Nf, double alpha);
std::string convertCharArrayToString(char* a, int size);
std::string convertCharVectorToString(std::vector<char> vector);

#endif
