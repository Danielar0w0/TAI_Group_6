#include "AbstractModelSerializer.h"

AbstractModelSerializer::AbstractModelSerializer(std::string modelPath) {
    this->modelPath = std::move(modelPath);
}

std::string AbstractModelSerializer::getModelPath() {
    return this->modelPath;
}


