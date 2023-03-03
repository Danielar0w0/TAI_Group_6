#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <cstring>
#include <set>
#include <fstream>

#include "utils/printUtils.h"

using namespace std;

// Smoothing parameter for estimating the probabilities
// int ALPHA = 2;

// Window size
// int K = 11;

// Threshold for the copy model
double THRESHOLD = 0.5;

// Develop a program, named cpm, that implements a copy model.
// All required parameters to control the model must be passed as command line arguments.

// One major advantage here is the ability to adjust the probabilistic model as the encoding proceeds,
// in response to the changing probability distribution from one encoded symbol to the next.

void getInputArguments(int argc, char *argv[], int &alpha, int &k, FILE* &target);
void copyModel(int k, int alpha, map<string, vector<int>> &sequences, vector<char> &alphabet, FILE *target);
set<char> obtainAlphabet();

void readFirstSequence(FILE* &target, char* &buffer, vector<char> &alphabet, int k);
bool readNextSequence(FILE* &target, char* &buffer, vector<char> &alphabet, int k);

char* getSequenceAtPosition(map<string, vector<int>> &sequences, int position);
void compareSequences(char* &buffer, char* &reference, int &Nh, int &Nf, int k);

void updateReferencePointer(map<string, vector<int>> &sequences, int &reference_pointer, char* &buffer);
double calculateHitProbability(int Nh, int Nf, int alpha);

void addLetterToAlphabet(vector<char> &alphabet, char c);
bool isNumber(char* s);

int main(int argc, char *argv[]) {

    int alpha = ALPHA; int k = K; // Default values
    FILE *target = nullptr; // Target sequence

    // Read command line arguments
    getInputArguments(argc, argv, alpha, k, target);

    cout << "Alpha: " << alpha << endl;
    cout << "k: " << k << endl << endl;

    // English alphabet (26 letters)
    // vector<char> alphabet;

    // Different sequences of symbols found in the target sequence
    // map<string, vector<int>> sequences;

    // Apply the copy model
    copyModel(k, alpha, sequences, alphabet, target);

    // printPositions(sequences, k);
    // printAlphabet(alphabet);
}

void getInputArguments(int argc, char *argv[], int &alpha, int &k, FILE* &target) {

    cout << "You have entered " << argc-1 << " arguments." << endl << endl;

    if (argc == 1) {
        cerr << "No arguments were entered." << endl;
        printHelp();
        exit(EXIT_FAILURE);
    } else {

        // Note: argc - 1 because the last argument can't be a flag
        for (int i = 0; i < argc - 1; ++i) {

            // Handle alpha
            if (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--alpha") == 0) {
                if (isNumber(argv[i + 1]))
                    alpha = atoi(argv[i+1]);

            // Handle k
            } else if (strcmp(argv[i], "-k") == 0) {
                if (isNumber(argv[i + 1]))
                    k = atoi(argv[i + 1]);

            // Handle target sequence
            } else if (strcmp(argv[i], "-f") == 0) {

                // Open the target sequence file
                target = fopen(argv[i + 1], "r");

                if (target == NULL) {
                    cerr << "Error reading text file" << target << ": " << strerror(errno) << endl;
                    exit(EXIT_FAILURE);
                }
            }
        }
    }

    // Check if file was provided
    if (target == nullptr) {
        cerr << "No target sequence was provided." << endl;
        exit(EXIT_FAILURE);
    }

}

void copyModel(int k, int alpha, map<string, vector<int>> &sequences, vector<char> &alphabet, FILE *target) {

    int target_pointer = 1; // Pointer to the current position in the target sequence
    int reference_pointer = 1; // Pointer to the current position in the reference sequence

    char* buffer = new char[k]; // Characters being read

    obtainAlphabet(alphabet, target);
    printAlphabet(alphabet);



    /*
    // Read the first k characters of the target sequence
    readFirstSequence(target, buffer, alphabet, k);

    // Add first buffer to the set of sequences
    string sequence = buffer;
    sequences[sequence].push_back(target_pointer++);

    // printSequence(buffer, k);

    int Tn = 0; // Number of times the copy model was used after the previous reposition
    int Nh = 0; // Number of hits
    int Nf = 0; // Number of prediction fails

    // Read the target sequence character by character
    while (readNextSequence(target, buffer, alphabet, k)) {

        char* reference = getSequenceAtPosition(sequences, reference_pointer);

        compareSequences(buffer, reference, Nh, Nf, k);

        cout << "Nh: " << Nh << endl;
        cout << "Nf: " << Nf << endl;

        // cout << "Hit probability: " << calculateHitProbability(Nh, Nf, alpha) << endl;
        if (calculateHitProbability(Nh, Nf, alpha) < THRESHOLD) {

            updateReferencePointer(sequences, reference_pointer, buffer);

            // Reset the counters
            Nh = 0; Nf = 0;
        }

        printSequence(buffer, k);
        cout << "Reference pointer: " << reference_pointer << endl;
        cout << "-------------------" << endl;

        // Add the buffer to the set of sequences
        sequence = buffer;
        sequences[sequence].push_back(target_pointer++);

        // printSequence(buffer, k);
    }
     */
}

set<char> obtainAlphabet(FILE* &target) {

    std::ifstream targetFile;
    targetFile.open("shopping_list.txt");

    vector<char> alphabet;
    char c;
    while((c = fgetc(target)) != EOF) {
        addLetterToAlphabet(alphabet, c);
    }

}

void readFirstSequence(FILE* &target, char* &buffer, vector<char> &alphabet, int k) {

    char c; // Current character in the target sequence

    // Read the first k characters of the target sequence
    for (int i = 0; i < k; ++i) {

        if ((c = fgetc(target)) == EOF) {
            cerr << "Target sequence is too short." << endl;
            exit(EXIT_FAILURE);
        }

        buffer[i] = c;

        // Add the letter to the alphabet
        addLetterToAlphabet(alphabet, c);
    }
}

bool readNextSequence(FILE* &target, char* &buffer, vector<char> &alphabet, int k) {

    char c = fgetc(target);
    if (c == EOF) {
        return false;
    }

    // Shift the buffer
    for (int i = 0; i < k-1; ++i) {
        buffer[i] = buffer[i+1];
    }

    // Add the character to the buffer
    buffer[k-1] = c;

    // Add the letter to the alphabet
    addLetterToAlphabet(alphabet, c);

    return true;
}

void addLetterToAlphabet(vector<char> &alphabet, char c) {

    // Check if the letter is already in the alphabet
    bool found = false;
    for (char i : alphabet) {
        if (i == c) {
            found = true;
            break;
        }
    }

    // Add the letter to the alphabet
    if (!found) {
        alphabet.push_back(c);
    }
}

char* getSequenceAtPosition(map<string, vector<int>> &sequences, int position) {

    for (auto &sequence : sequences) {
        for (int i : sequence.second) {
            if (i == position) {
                return (char*) sequence.first.c_str();
            }
        }
    }
    return nullptr;
}

void compareSequences(char* &buffer, char* &reference, int &Nh, int &Nf, int k) {

    // Compare the two sequences
    for (int i = 0; i < k; ++i) {
        if (buffer[i] == reference[i]) Nh++; else Nf++;
    }
}

double calculateHitProbability(int Nh, int Nf, int alpha) {
    return (double) (Nh + alpha) / (Nh + Nf + 2*alpha);
}

void updateReferencePointer(map<string, vector<int>> &sequences, int &reference_pointer, char* &buffer) {

        // Check if the buffer is in the set of sequences
        string sequence = buffer;
        if (sequences.find(sequence) != sequences.end()) {
            // Update the reference pointer (last occurrence)
            reference_pointer = sequences[sequence].back();
        }

        cout << "Repositioned." << endl;
}

bool isNumber(char* s) {

    for (int i = 0; i < strlen(s); ++i) {
        if (!isdigit(s[i])) {
            return false;
        }
    }
    return true;
}