#ifndef SRC_MODELSERIALIZERHANDLER_H
#define SRC_MODELSERIALIZERHANDLER_H

#include <string>

#include "../utils/ModelType.h"
#include "../abstract/AbstractModelSerializer.h"
#include "../implementation/PositionalModelSerializer.h"
#include "../implementation/ProbabilisticModelSerializer.h"

class ModelSerializerHandler {

    public:

        explicit ModelSerializerHandler();

        static ModelType getModelType(const std::string& modelPath);

};


#endif //SRC_MODELSERIALIZERHANDLER_H
