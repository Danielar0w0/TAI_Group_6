#include "GeneratorInputArguments.h"
#include "../../utils/generalUtils.h"

#include <utility>
#include <iostream>

GeneratorInputArguments::GeneratorInputArguments() = default;

std::string GeneratorInputArguments::getModelPath() const {
    return this->modelPath;
}

int GeneratorInputArguments::getAmountOfCharactersToGenerate() const {
    return this->amountOfCharactersToGenerate;
}

void GeneratorInputArguments::printUsage() {
    std::cout << "gen: gen [-mf]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "-m \t Model file path" << std::endl;
    std::cout << "-g \t Amount of characters to generate" << std::endl;
    std::cout << "-i \t Interactive mode" << std::endl;
    std::cout << "-l \t Logging level" << std::endl;
    std::cout << "-h \t Print usage" << std::endl;
}

void GeneratorInputArguments::parseArguments(int argc, char **argv) {

    std::cout << "You have entered " << argc - 1 << " arguments." << std::endl;

    if (argc == 1) {
        std::cerr << "No arguments were entered." << std::endl;
        GeneratorInputArguments::printUsage();
        exit(EXIT_FAILURE);
    } else {

        for (int i = 0; i < argc; ++i) {
            if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--interactive") == 0) {
                this->interactive = true;
            } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
                GeneratorInputArguments::printUsage();
                exit(EXIT_SUCCESS);
            }
        }

        // Note: argc - 1 because the last argument can't be a flag
        for (int i = 0; i < argc - 1; ++i) {
            if (strcmp(argv[i], "-m") == 0 || strcmp(argv[i], "--model") == 0) {
                this->modelPath = argv[i + 1];
            } else if (strcmp(argv[i], "-g") == 0 || strcmp(argv[i], "--generationCharacters") == 0) {
                if (isNumber(argv[i + 1]))
                    this->amountOfCharactersToGenerate = atoi(argv[i + 1]);
            } else if (strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--logging") == 0) {
                if (isNumber(argv[i + 1]))
                    this->loggingLevel = atoi(argv[i + 1]);
            }
        }

        if (this->amountOfCharactersToGenerate == 0) {
            std::cout << "Amount of Characters to generate not provided. Using default value: 50" << std::endl;
            amountOfCharactersToGenerate = 50;
        }

    }


}

bool GeneratorInputArguments::checkArguments() const {

    if (this->modelPath.empty()) {
        std::cerr << "The Path of the model file was not provided." << std::endl;
        return false;
    }

    return true;

}

int GeneratorInputArguments::getLoggingLevel() const {
    return this->loggingLevel;
}

int GeneratorInputArguments::isInteractive() const {
    return this->interactive;
}
