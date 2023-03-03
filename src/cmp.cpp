#include <iostream>
#include <string>
#include <cstring>
#include <set>

#include "utils/printUtils.h"
#include "input/Input.h"

using namespace std;

// Develop a program, named cpm, that implements a copy model.
// All required parameters to control the model must be passed as command line arguments.

// One major advantage here is the ability to adjust the probabilistic model as the encoding proceeds,
// in response to the changing probability distribution from one encoded symbol to the next.

// Threshold for the copy model
double THRESHOLD = 0.5;

Input getInputArguments(int argc, char *argv[]);

FILE *obtainTarget(string filePath);

set<char> obtainAlphabet(string filePath);

bool isNumber(char *s);

int main(int argc, char *argv[]) {

    // Read command line arguments
    Input input = getInputArguments(argc, argv);

    int alpha = input.getAlpha();       // Alpha (!= 0)
    int k = input.getK();               // Window size

    // Check if file was provided
    string filePath = input.getPath();
    if (filePath.empty()) {
        cerr << "No target sequence was provided." << endl;
        exit(EXIT_FAILURE);
    }

    // Get alphabet
    set<char> alphabet = obtainAlphabet(filePath);
    printAlphabet(alphabet);




    // Apply the copy model
    // copyModel(k, alpha, sequences, alphabet, target);

    // printPositions(sequences, k);
    // printAlphabet(alphabet);
}

Input getInputArguments(int argc, char *argv[]) {

    cout << "You have entered " << argc - 1 << " arguments." << endl << endl;

    if (argc == 1) {
        cerr << "No arguments were entered." << endl;
        printHelp();

        exit(EXIT_FAILURE);

    } else {

        int alpha, k; string filePath;

        // Note: argc - 1 because the last argument can't be a flag
        for (int i = 0; i < argc - 1; ++i) {

            // Handle alpha
            if (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--alpha") == 0) {
                if (isNumber(argv[i + 1]))
                    alpha = atoi(argv[i + 1]);

            // Handle k
            } else if (strcmp(argv[i], "-k") == 0) {
                if (isNumber(argv[i + 1]))
                    k = atoi(argv[i + 1]);

            // Handle file path (target sequence)
            } else if (strcmp(argv[i], "-f") == 0) {
                filePath = argv[i + 1];
            }
        }

        return Input(filePath, alpha, k);
    }
}

FILE* obtainTarget(string filePath) {

    // Open the target sequence file
    FILE *target = fopen(filePath.c_str(), "r");

    if (target == NULL) {
        cerr << "Error reading text file" << target << ": " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }

    return target;
}

set<char> obtainAlphabet(string filePath) {

    FILE* target = obtainTarget(filePath);

    set<char> alphabet; char c;
    while((c = fgetc(target)) != EOF) {
        alphabet.insert(c);
    }

    fclose(target);
    return alphabet;
}

bool isNumber(char *s) {

    for (int i = 0; i < strlen(s); ++i) {
        if (!isdigit(s[i])) {
            return false;
        }
    }
    return true;
}

