#include <string>

#include "ModelType.h"

std::string modelTypeToString(ModelType modelType) {
    switch (modelType) {
        case POSITIONAL:
            return "Positional";
        case PROBABILISTIC:
            return "Probabilistic";
        default:
            return "Unknown";
    }
}
