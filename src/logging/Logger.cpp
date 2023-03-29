#include "Logger.h"

#include <iostream>

Logger::Logger() {
    loggingLevel = LoggingLevel::INFO;
}

void Logger::setLevel(int level) {
    this->loggingLevel = level;
}

int Logger::getLevel() const {
    return loggingLevel;
}

void Logger::debug(const std::string& message) const {

    if (loggingLevel >= LoggingLevel::DEBUG) {
        std::cout << message << std::endl;
    }

}

void Logger::info(const std::string& message) const {

    if (loggingLevel >= LoggingLevel::INFO) {
        std::cout << message << std::endl;
    }

}

void Logger::error(const std::string& message) const {

    if (loggingLevel >= LoggingLevel::ERROR) {
        std::cerr << message << std::endl;
    }

}

