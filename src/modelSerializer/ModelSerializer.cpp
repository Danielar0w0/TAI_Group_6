#include "ModelSerializer.h"

#include <utility>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

ModelSerializer::ModelSerializer() = default;

ModelSerializer::ModelSerializer(map<string, map<char, double>> model) {
    this->model = std::move(model);
}

int ModelSerializer::outputModel(const string& outputPath) {
    // Check if model is not empty
    if(this->model.empty()) {
        cerr << "Cannot output model because model is empty." << endl;
        return 1;
    }

    // Create file
    FILE* target = fopen(outputPath.c_str(), "w");

    if (target == nullptr) {
        cerr << "Error creating text file " << outputPath.c_str() << ": " << stderr << endl;
        return 1;
    }

    // Iterate over map
    for (const auto& i: this->model) {
        string sequence = i.first;
        for (auto j: i.second) {
            char c = j.first;
            double prob = j.second;
            int written = fprintf(target,"%s;%c;%f\n", sequence.c_str(), c, prob);
            if(written < 0) {
                cerr << "Error writing to text file " << outputPath.c_str() << ": " << stderr << endl;
                return 1;
            }
        }
    }

    fclose(target);
    return 0;
}

int ModelSerializer::loadModel(const string& inputPath) {
    // Check if model is empty, if not, empty it
    if(!this->model.empty()) this->model.clear();

    // Open file
    fstream file;
    file.open(inputPath.c_str(), ios::in);
    if(file.is_open()) {
        string line;
        int lineCount = 0;
        while(getline(file, line)) { // Read file line by line
            // Parse obtained line
            string sequence;
            char c;
            double prob;
            // Get sequence
            size_t pos = line.find(';');
            if(pos == std::string::npos) {
                cerr << "Error parsing text file in line: " << lineCount << ": wrong model format." << endl;
                return 1;
            }
            sequence = line.substr(0, pos);
            line.erase(0, pos + 1); // erase sequence and delimiter from line

            // Get char
            pos = line.find(';');
            if(pos == std::string::npos) {
                cerr << "Error parsing text file in line: " << lineCount << ": wrong model format." << endl;
                return 1;
            }
            c = line.substr(0, pos).at(0);
            line.erase(0, pos + 1); // erase char and delimiter from line

            // Get double
            prob = stod(line); // TODO: handle possible exception

            // Insert obtained values in model
            this->model[sequence][c] = prob;

            lineCount++;
        }
        file.close();
    }
    else {
        cerr << "Error opening text file " << inputPath.c_str() << ": " << stderr << endl;
        return 1;
    }
    return 0;
}

void ModelSerializer::printModel(int lim = -1) {
    int cnt = 0;
    for (const auto& i: this->model) {
        string sequence = i.first;
        printf("%s:\n",sequence.c_str());
        for (auto j: i.second) {
            char c = j.first;
            double prob = j.second;
            printf("\t%c: %f\n", c, prob);
        }
        cnt++;
        if(lim > 0 && cnt >= lim) break;
    }
}

map<string, map<char, double>> ModelSerializer::getModel() {
    return this->model;
}


