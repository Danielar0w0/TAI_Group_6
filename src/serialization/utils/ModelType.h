#ifndef SRC_MODELTYPE_H
#define SRC_MODELTYPE_H

enum ModelType : short {
    POSITIONAL = 0,
    PROBABILISTIC = 1,
    UNKNOWN = 2
};

std::string modelTypeToString(ModelType modelType);

#endif
