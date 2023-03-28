#ifndef SRC_LOGGABLEENTITY_H
#define SRC_LOGGABLEENTITY_H


#include "../logging/Logger.h"

class LoggableEntity {

protected:

    Logger logger;

public:

    virtual ~LoggableEntity() = default;

    void setLogger(const Logger &loggerInstance);
    Logger getLogger();

};


#endif
