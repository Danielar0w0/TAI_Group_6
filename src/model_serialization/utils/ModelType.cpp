#include <string>

#include "ModelType.h"

std::string modelTypeToString(ModelType modelType) {
    switch (modelType) {
        case SEQUENTIAL:
            return "Sequential";
        case PROBABILISTIC:
            return "Probabilistic";
        default:
            return "Unknown";
    }
}
