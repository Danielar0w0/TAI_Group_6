#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <cstring>

using namespace std;

// Develop a program, named cpm, that implements a copy model.
// All required parameters to control the model must be passed as command line arguments.

// One major advantage here is the ability to adjust the probabilistic model as the encoding proceeds,
// in response to the changing probability distribution from one encoded symbol to the next.

void print_sequences(map<string, vector<int>> sequences, int k);
void print_buffer(char* buffer, int k);

bool is_number(char* s);

int main(int argc, char *argv[]) {

    int alpha = 0; // “Smoothing” parameter for estimating the probabilities (default value)
    int k = 11; // Window size (default value)

    FILE *target = nullptr; // Target sequence

    cout << "You have entered " << argc-1 << " arguments." << endl << endl;
    if (argc == 1) {
        cerr << "No arguments were entered." << endl;
        exit(EXIT_FAILURE);

    } else if (argc == 6+1) {

        // Note: argc - 1 because the last argument can't be a flag
        for (int i = 0; i < argc - 1; ++i) {

            // Handle alpha
            if (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--alpha") == 0) {
                if (is_number(argv[i+1]))
                    alpha = atoi(argv[i+1]);

            // Handle k
            } else if (strcmp(argv[i], "-k") == 0) {
                if (is_number(argv[i + 1]))
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

    int Nh; // Number of hits
    int Nf; // Number of prediction fails

    int target_pointer = 0; // Pointer to the current position in the target sequence
    int reference_pointer = 0; // Pointer to the current position in the reference sequence

    char c; // Current character in the target sequence
    char* buffer = new char[k]; // Characters being read

    // Set of all different sequences of symbols, found in the target sequence
    map<string, vector<int>> sequences;

    // Read the first k characters of the target sequence
    for (int i = 0; i < k; ++i) {

        if ((c = fgetc(target)) == EOF) {
            cerr << "Target sequence is too short." << endl;
            exit(EXIT_FAILURE);
        }

        buffer[i] = c;
    }

    // Add first buffer to the set of sequences
    string current_buffer = buffer;
    sequences[current_buffer].push_back(target_pointer++);

    // Read the target sequence character by character
    while ((c = fgetc(target)) != EOF) {

        // TODO: Implement the copy model

        // Shift the buffer
        for (int i = 0; i < k-1; ++i) {
            buffer[i] = buffer[i+1];
        }

        // Add the character to the buffer
        buffer[k-1] = c;

        // Add the buffer to the set of sequences
        current_buffer = buffer;
        sequences[current_buffer].push_back(target_pointer++);

        // print_buffer(buffer, k);
    }

    print_sequences(sequences, k);
}

void print_sequences(map<string, vector<int>> sequences, int k) {

    // Print the number of sequences
    cout << "Number of sequences: " << sequences.size() << endl;

    for (auto& t : sequences) {

        char* sequence = new char[k];
        strcpy(sequence, t.first.c_str());

        print_buffer(sequence, k);
        cout << "Positions: ";
        for (auto& i : t.second)
            cout << i << " ";
        cout << endl;
    }
}

void print_buffer(char* buffer, int k) {
    for (int i = 0; i < k; ++i) {
        cout << buffer[i];
    }
    cout << endl;
}

bool is_number(char* s) {

    for (int i = 0; i < strlen(s); ++i) {
        if (!isdigit(s[i])) {
            return false;
        }
    }
    return true;
}