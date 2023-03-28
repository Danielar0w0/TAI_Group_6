#ifndef SRC_ABSTRACTINPUTARGUMENTS_H
#define SRC_ABSTRACTINPUTARGUMENTS_H


class AbstractInputArguments {

public:

    AbstractInputArguments() = default;

    virtual void parseArguments(int argc, char *argv[]) = 0;
    [[nodiscard]] virtual bool checkArguments() const = 0;

};


#endif
