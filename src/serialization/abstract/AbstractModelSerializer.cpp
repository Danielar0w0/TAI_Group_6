#include "AbstractModelSerializer.h"

AbstractModelSerializer::AbstractModelSerializer(std::string modelPath) {
    this->modelPath = std::move(modelPath);
}

std::string AbstractModelSerializer::getModelPath() {
    return this->modelPath;
}

std::string AbstractModelSerializer::getInputFilePath() {
    return this->inputFilePath;
}

void AbstractModelSerializer::setInputFilePath(const std::string& inputPath) {
    this->inputFilePath = inputPath;
}


