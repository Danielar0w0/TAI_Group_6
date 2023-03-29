#ifndef SRC_LOGGER_H
#define SRC_LOGGER_H

#include <string>

enum LoggingLevel : short;
enum LoggingLevel : short {
    ERROR = 0,
    INFO = 1,
    DEBUG = 2
};

class Logger {

private:
    int loggingLevel;

public:

    Logger();

    void setLevel(int level);
    int getLevel() const;

    void debug(const std::string& message) const;
    void info(const std::string& message) const;
    void error(const std::string& message) const;

};

#endif //SRC_LOGGER_H
