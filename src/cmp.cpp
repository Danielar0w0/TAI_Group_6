#include <iostream>
#include <string>
#include <cstring>
#include <set>

#include "utils/printUtils.h"
#include "utils/readTarget.h"
#include "input/Input.h"

using namespace std;

// Develop a program, named cpm, that implements a copy model.
// All required parameters to control the model must be passed as command line arguments.

// One major advantage here is the ability to adjust the probabilistic model as the encoding proceeds,
// in response to the changing probability distribution from one encoded symbol to the next.

// Threshold for the copy model
double THRESHOLD = 0.5;

Input getInputArguments(int argc, char *argv[]);
set<char> obtainAlphabet(string filePath);

int updateReferencePointer(map<string, vector<int>> &sequences, string &sequence);
double calculateHitProbability(int Nh, int Nf, int alpha);

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

    // First Pass: Get alphabet
    set<char> alphabet = obtainAlphabet(filePath);
    printAlphabet(alphabet); cout << endl;

    // Second Pass
    map<string, vector<int>> positions;
    string reference; string sequence;

    FILE* target = obtainTarget(filePath);
    char* buffer = readFirstSequence(target, k);
    // printSequence(buffer, k);

    // Start reference sequence
    for (int i = 0; i < k; ++i) {
        reference += buffer[i];
    }

    int targetPointer = 1; // Pointer to the current position in the target sequence
    map<string, int> referencePointers; // Pointers to past positions in the reference sequence

    while (readNextSequence(target, buffer, k)) {

        // printSequence(buffer, k);
        // Add new character to reference sequence
        reference += buffer[k-1];

        // Update current sequence
        sequence = buffer[0];
        for (int i = 1; i < k; ++i) {
            sequence += buffer[i];
        }

        // If no reference pointer yet, try to update it
        if (referencePointers.find(sequence) == referencePointers.end()) {
            // Update reference pointer
            int newPointer = updateReferencePointer(positions, sequence);
            if (newPointer != -1) {
                referencePointers[sequence] = newPointer;
                cout << "Current position: [" << targetPointer << "]" << endl;
                cout << "Repositioned reference: [" << newPointer << "]" << endl;
            }
        }

        // If reference pointer exists, predict
        if (referencePointers.find(sequence) != referencePointers.end()) {

            /*
            int Tn = 0; // Number of times the copy model was used after the previous reposition
            int Nh = 0; // Number of hits
            int Nf = 0; // Number of prediction fails
             */

            // TODO: Keep track of Nh and Nf for all sequences
            map<char, int> Nh; // Number of hits
            map<char, int> Nf; // Number of prediction fails

            // Read next character (prediction)
            char prediction = reference[referencePointers[sequence] + k];

            // Check number of hits/fails for each character in the alphabet
            for (char c : alphabet) {
                // Check if the prediction is correct
                if (prediction == c) {
                    Nh[c]++;
                } else {
                    Nf[c]++;
                }
            }

            cout << "Sequence: " << sequence << endl;
            cout << "Prediction: " << prediction << endl;

            cout << "Hit probability for prediction " << prediction << ": " <<
                 calculateHitProbability(Nh[prediction], Nf[prediction], alpha) << endl;
            cout << endl;

            // If prediction doesn't meet the threshold, reposition
            if (calculateHitProbability(Nh[prediction], Nf[prediction], alpha) < THRESHOLD) {

                // Update reference pointer
                int newPointer = updateReferencePointer(positions, sequence);
                if (newPointer != -1) {
                    referencePointers[sequence] = newPointer;
                    cout << "Repositioned: [" << newPointer << "]" << endl;
                }

                // Reset the counters
                Nh.clear();
                Nf.clear();
            }
        }

        // Add sequence position and update target pointer
        positions[sequence].push_back(targetPointer++);

        // cout << "Reference: " << reference << endl;
    }

    // printPositions(positions, k);
    // printReferencePointers(referencePointers, k);
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

        if (alpha == 0) {
            cout << "Alpha was not provided. Using default value: 2" << endl;
            alpha = 1;
        }

        if (k == 0) {
            cout << "Window size was not provided. Using default value: 3" << endl;
            k = 3;
        }

        return Input(filePath, alpha, k);
    }
}

set<char> obtainAlphabet(string filePath) {

    FILE* target = obtainTarget(filePath);
    set<char> alphabet; int length = 0;

    char c;
    while((c = fgetc(target)) != EOF) {
        alphabet.insert(c);
        length++;
    }

    fclose(target);

    return alphabet;
}

int updateReferencePointer(map<string, vector<int>> &sequences, string &sequence) {

    int referencePointer = -1;

    // Check if the buffer is in the set of sequences
    if (sequences.find(sequence) != sequences.end()) {
        // Update the reference pointer (last occurrence)
        referencePointer = sequences[sequence].back();
    }

    return referencePointer;
}

double calculateHitProbability(int Nh, int Nf, int alpha) {
    return (double) (Nh + alpha) / (Nh + Nf + 2*alpha);
}

bool isNumber(char *s) {

    for (int i = 0; i < strlen(s); ++i) {
        if (!isdigit(s[i])) {
            return false;
        }
    }
    return true;
}

