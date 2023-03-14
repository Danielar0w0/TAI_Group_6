#include "GeneratorInputArguments.h"

#include <utility>
#include <iostream>

GeneratorInputArguments::GeneratorInputArguments(std::string modelPath, int generationAmount) {
    this->modelPath = std::move(modelPath);
    this->amountOfCharactersToGenerate = generationAmount;
}

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
}
