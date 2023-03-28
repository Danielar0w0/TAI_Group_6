#include <utility>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <random>

#include "PositionalModelSerializer.h"
#include "../utils/ModelType.h"

bool PositionalModelSerializer::outputModel() {

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

    int writeStatus = fprintf(target, "Model Type - %s:%i\n", modelTypeToString(ModelType::POSITIONAL).c_str(), ModelType::POSITIONAL);

    if (writeStatus < 0) {
        std::cerr << "Error writing to text file " << this->modelPath.c_str() << ": " << stderr << std::endl;
        return false;
    }

    writeStatus = fprintf(target, "Original File:%s\n", this->inputFilePath.c_str());

    if (writeStatus < 0) {
        std::cerr << "Error writing to text file " << this->modelPath.c_str() << ": " << stderr << std::endl;
        return false;
    }

    // Iterate over map
    for (const auto& i: this->model) {

        std::string window = i.first;
        std::vector<int> positions = i.second;

        writeStatus = fprintf(target,"%s;", window.c_str());

        if (writeStatus < 0) {
            std::cerr << "Error writing to text file " << this->modelPath.c_str() << ": " << stderr << std::endl;
            return false;
        }

        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist6(1, std::min((int)positions.size(), positionsPerSequenceLimit));

        for (int position : positions) {

            writeStatus = fprintf(target, "%i,", position);

            if (writeStatus < 0) {
                std::cerr << "Error writing to text file " << this->modelPath.c_str() << ": " << stderr << std::endl;
                return false;
            }

        }

        writeStatus = fprintf(target, "\n");

        if (writeStatus < 0) {
            std::cerr << "Error writing to text file " << this->modelPath.c_str() << ": " << stderr << std::endl;
            return false;
        }

    }

    fclose(target);

    return true;

}

bool PositionalModelSerializer::loadModel() {

    // Check if model is empty, if not, empty it
    if(!this->model.empty()) {
        this->model.clear();
    }

    // Open file
    std::fstream file;
    file.open(this->modelPath.c_str(), std::ios::in);

    if (file.is_open()) {

        std::string line;
        int lineCount = 0;

        getline(file, line); // Ignore first line

        getline(file, line); // Read second line - i.e., path for original file

        size_t pos0 = line.find(':');

        if(pos0 == std::string::npos) {
            std::cerr << "Error parsing text file in line: " << lineCount << ": wrong model format." << std::endl;
            return false;
        }

        std::string originalFilePath = line.substr(pos0 + 1, line.length() - 1);
        this->inputFilePath = originalFilePath;

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

            pos = sequence.find(',');

            if (pos == std::string::npos) {
                std::cerr << "Error parsing text file in line: " << lineCount << ": wrong model format." << std::endl;
                return false;
            }

            std::vector<int> sequenceVector;

            while (pos != std::string::npos) {
                sequenceVector.push_back(std::stoi(sequence.substr(0, pos)));
                sequence.erase(0, pos + 1);
                pos = sequence.find(',');
            }

            // Insert obtained values in model
            this->model[window] = sequenceVector;

            lineCount++;

        }

        file.close();

    } else {
        std::cerr << "Error opening text file " << this->modelPath.c_str() << ": " << stderr << std::endl;
        return false;
    }

    return true;

}

std::map<std::string, std::vector<int>> PositionalModelSerializer::getModel() {
    return this->model;
}

void PositionalModelSerializer::setModel(std::map<std::string, std::vector<int>> modelToSet) {
    this->model = std::move(modelToSet);
}

