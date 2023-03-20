#include <utility>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "SequentialModelSerializer.h"
#include "../utils/ModelType.h"

bool SequentialModelSerializer::outputModel() {

    // Check if model is not empty
    if(this->model.empty()) {
        std::cerr << "Cannot output model because model is empty." << std::endl;
        return false;
    }

    // Create file
    FILE* target = fopen(this->modelPath.c_str(), "w");

    if (target == nullptr) {
        std::cerr << "Error creating text file " << this->modelPath.c_str() << ": " << stderr << std::endl;
        return false;
    }

    int writeStatus = fprintf(target, "Model Type - %s:%i\n", modelTypeToString(ModelType::SEQUENTIAL).c_str(), ModelType::SEQUENTIAL);

    if (writeStatus < 0) {
        std::cerr << "Error writing to text file " << this->modelPath.c_str() << ": " << stderr << std::endl;
        return false;
    }

    // Iterate over map
    for (const auto& i: this->model) {

        std::string window = i.first;
        std::string sequence = i.second;

        writeStatus = fprintf(target,"%s;%s\n", window.c_str(), sequence.c_str());

        if (writeStatus < 0) {
            std::cerr << "Error writing to text file " << this->modelPath.c_str() << ": " << stderr << std::endl;
            return false;
        }

    }

    fclose(target);
    return true;

}

bool SequentialModelSerializer::loadModel() {

    // Check if model is empty, if not, empty it
    if(!this->model.empty()) {
        this->model.clear();
    }

    // Open file
    std::fstream file;
    file.open(this->modelPath.c_str(), std::ios::in);

    if(file.is_open()) {

        std::string line;
        int lineCount = 0;

        getline(file, line); // Ignore first line

        while(getline(file, line)) { // Read file line by line

            // Parse obtained line
            size_t pos;

            // Get sequence
            pos = line.find(';');
            if(pos == std::string::npos) {
                std::cerr << "Error parsing text file in line: " << lineCount << ": wrong model format." << std::endl;
                return false;
            }

            std::string window = line.substr(0, pos);
            std::string sequence = line.substr(pos + 1, line.length() - 1);

            line.erase(0, pos + 1); // erase sequence and delimiter from line

            // Insert obtained values in model
            this->model[window] = sequence;

            lineCount++;
        }

        file.close();

    } else {
        std::cerr << "Error opening text file " << this->modelPath.c_str() << ": " << stderr << std::endl;
        return false;
    }

    return true;

}

std::map<std::string, std::string> SequentialModelSerializer::getModel() {
    return this->model;
}

void SequentialModelSerializer::setModel(std::map<std::string, std::string> modelToSet) {
    this->model = std::move(modelToSet);
}

