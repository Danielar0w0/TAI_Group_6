#include <iostream>
#include <fstream>

#include "ModelSerializerHandler.h"

ModelSerializerHandler::ModelSerializerHandler() = default;

ModelType ModelSerializerHandler::getModelType(const std::string& modelPath) {

    std::fstream file;
    file.open(modelPath.c_str(), std::ios::in);

    if (!file.is_open()) {
        std::cerr << "Error opening file " << modelPath.c_str() << ": " << stderr << std::endl;
        return ModelType::UNKNOWN;
    }

    std::string firstLine;

    getline(file, firstLine);

    if (firstLine.empty()) {
        std::cerr << "Unable to parse Model Type - No Matching Line Found." << std::endl;
        return ModelType::UNKNOWN;
    }

    size_t pos = firstLine.find(':');

    if (pos == std::string::npos) {
        std::cerr << "Unable to parse Model Type - Wrong Format." << std::endl;
        return ModelType::UNKNOWN;
    }

    std::string modelType = firstLine.substr(pos+1, firstLine.length() - 1);

    if (std::stoi(modelType) == ModelType::POSITIONAL) {
        return ModelType::POSITIONAL;
    } else if (std::stoi(modelType) == ModelType::PROBABILISTIC) {
        return ModelType::PROBABILISTIC;
    }

    file.close();

    return ModelType::UNKNOWN;

}
