#ifndef SRC_READTARGET_H
#define SRC_READTARGET_H

#include <cstdio>
#include <cstdlib>
#include <iostream>

using namespace std;

FILE* obtainTarget(string filePath) {

    // Open the target sequence file
    FILE *target = fopen(filePath.c_str(), "r");

    if (target == NULL) {
        cerr << "Error reading text file" << target << ": " << stderr << endl;
        exit(EXIT_FAILURE);
    }

    return target;
}

char* readFirstSequence(FILE* &target, int k) {

    char* buffer = new char[k]; // Current sequence
    char c; // Current character in the target sequence

    // Read the first k characters of the target sequence
    for (int i = 0; i < k; ++i) {

        if ((c = fgetc(target)) == EOF) {
            cerr << "Target sequence is too short." << endl;
            exit(EXIT_FAILURE);
        }

        buffer[i] = c;
    }

    return buffer;
}

bool readNextSequence(FILE* &target, char* &buffer, int k) {

    char c = fgetc(target);
    if (c == EOF) {
        return false;
    }

    while (c == 0x20 || c == 0x0A || c == 0x0D) {
        c = fgetc(target);
        if (c == EOF) {
            return false;
        }
    }

    // Shift the buffer
    for (int i = 0; i < k-1; ++i) {
        buffer[i] = buffer[i+1];
    }

    // Add the character to the buffer
    buffer[k-1] = c;

    return true;
}

#endif //SRC_READTARGET_H
