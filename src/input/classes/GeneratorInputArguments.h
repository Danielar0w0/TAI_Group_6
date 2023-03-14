#ifndef SRC_GENERATORINPUTARGUMENTS_H
#define SRC_GENERATORINPUTARGUMENTS_H


#include <string>

class GeneratorInputArguments {

    std::string modelPath;
    int amountOfCharactersToGenerate;

    public:

        explicit GeneratorInputArguments(std::string modelPath, int amountOfCharactersToGenerate);

        [[nodiscard]] std::string getModelPath() const;
        [[nodiscard]] int getAmountOfCharactersToGenerate() const;

        static void printUsage();

};


#endif
