#include <utility>
#include <string>
#include <iostream>
#include <fstream>

#include "ProbabilisticModelSerializer.h"
#include "../utils/ModelType.h"

using namespace std;

bool ProbabilisticModelSerializer::outputModel() {

    // Check if model is not empty
    if(this->model.empty()) {
        cerr << "Cannot output model because model is empty." << endl;
        return false;
    }

    // Create file
    FILE* target = fopen(this->modelPath.c_str(), "w");

    if (target == nullptr) {
        cerr << "Error creating text file " << this->modelPath.c_str() << ": " << stderr << endl;
        return false;
    }

    int writeStatus = fprintf(target, "Model Type - %s:%i\n", modelTypeToString(ModelType::PROBABILISTIC).c_str(), ModelType::PROBABILISTIC);

    if(writeStatus < 0) {
        cerr << "Error writing to text file " << this->modelPath.c_str() << ": " << stderr << endl;
        return false;
    }

    writeStatus = fprintf(target, "Original File:%s\n", this->inputFilePath.c_str());

    if(writeStatus < 0) {
        cerr << "Error writing to text file " << this->modelPath.c_str() << ": " << stderr << endl;
        return false;
    }

    // Iterate over map
    for (const auto& i: this->model) {
        string sequence = i.first;
        for (auto j: i.second) {
            char c = j.first;
            double prob = j.second;
            writeStatus = fprintf(target,"%s;%c;%f\n", sequence.c_str(), c, prob);
            if(writeStatus < 0) {
                cerr << "Error writing to text file " << this->modelPath.c_str() << ": " << stderr << endl;
                return false;
            }
        }
    }

    fclose(target);
    return true;

}

bool ProbabilisticModelSerializer::loadModel() {

    // Check if model is empty, if not, empty it
    if(!this->model.empty()) this->model.clear();

    // Open file
    fstream file;
    file.open(this->modelPath.c_str(), ios::in);

    if (file.is_open()) {

        string line;
        int lineCount = 0;

        getline(file, line); // Ignore first line

        getline(file, line); // Read second line - i.e., path for original file

        size_t pos0 = line.find(':');

        if(pos0 == std::string::npos) {
            cerr << "Error parsing text file in line: " << lineCount << ": wrong model format." << endl;
            return false;
        }

        string originalFilePath = line.substr(pos0 + 1, line.length() - 1);
        this->inputFilePath = originalFilePath;

        while(getline(file, line)) { // Read file line by line
            // Parse obtained line
            string sequence;
            char c;
            double prob;
            // Get sequence
            size_t pos = line.find(';');
            if(pos == std::string::npos) {
                cerr << "Error parsing text file in line: " << lineCount << ": wrong model format." << endl;
                return false;
            }
            sequence = line.substr(0, pos);
            line.erase(0, pos + 1); // erase sequence and delimiter from line

            // Get char
            pos = line.find(';');
            if(pos == std::string::npos) {
                cerr << "Error parsing text file in line: " << lineCount << ": wrong model format." << endl;
                return false;
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

    } else {
        cerr << "Error opening text file " << this->modelPath.c_str() << ": " << stderr << endl;
        return false;
    }

    return true;

}

map<string, map<char, double>> ProbabilisticModelSerializer::getModel() {
    return this->model;
}

void ProbabilisticModelSerializer::setModel(std::map<std::string, std::map<char, double>> modelToSet) {
    this->model = std::move(modelToSet);
}


