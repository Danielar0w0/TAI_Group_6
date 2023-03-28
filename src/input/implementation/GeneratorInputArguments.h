#ifndef SRC_GENERATORINPUTARGUMENTS_H
#define SRC_GENERATORINPUTARGUMENTS_H


#include <string>
#include "../abstract/AbstractInputArguments.h"

class GeneratorInputArguments : public AbstractInputArguments {

    std::string modelPath;
    int amountOfCharactersToGenerate = 50;
    int loggingLevel = 1;
    bool interactive = false;

    public:

        explicit GeneratorInputArguments();

        [[nodiscard]] std::string getModelPath() const;
        [[nodiscard]] int getAmountOfCharactersToGenerate() const;
        [[nodiscard]] int getLoggingLevel() const;
        [[nodiscard]] int isInteractive() const;

        void parseArguments(int argc, char *argv[]) override;

        // Don't add nodiscard - I may want to check the arguments and do nothing with the return value.
        bool checkArguments() const override;

        static void printUsage();

};


#endif
