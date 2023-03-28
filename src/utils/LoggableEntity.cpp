#include "LoggableEntity.h"
#include "../logging/Logger.h"

void LoggableEntity::setLogger(const Logger &loggerInstance) {
    this->logger = loggerInstance;
}

Logger LoggableEntity::getLogger() {
    return this->logger;
}
