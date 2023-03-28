#ifndef SRC_ABSTRACTGENERATOR_H
#define SRC_ABSTRACTGENERATOR_H


#include "../../serialization/abstract/AbstractModelSerializer.h"
#include "../../utils/LoggableEntity.h"

class AbstractGenerator : public LoggableEntity {

public:

    virtual void generateTextInteractively(int generationSize) = 0;
    virtual void generateTextOnce(int generationSize) = 0;
    virtual int getModelWindowSize() = 0;
    void printHeader();

};


#endif
