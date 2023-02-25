#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <cstring>

using namespace std;

#ifndef SRC_PRINTUTILS_H
#define SRC_PRINTUTILS_H

void printSequence(char* sequence, int k) {
    cout << "Sequence: ";
    for (int i = 0; i < k; ++i) {
        cout << sequence[i];
    }
    cout << endl;
}

void printPositions(map<string, vector<int>> positions, int k) {
    cout << "Number of sequences: " << positions.size() << endl;
    for (auto& t : positions) {

        char* sequence = new char[k];
        strcpy(sequence, t.first.c_str());

        printSequence(sequence, k);
        cout << "Positions: ";
        for (int i : t.second)
            cout << i << " ";
        cout << endl;
    }
    cout << endl;
}

void printAlphabet(vector<char> alphabet) {
    cout << "Alphabet: ";
    for (char c : alphabet)
        cout << c << " ";
    cout << endl;
}

#endif //SRC_PRINTUTILS_H
