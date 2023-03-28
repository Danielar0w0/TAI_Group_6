#include "PositionalGenerator.h"

#include <utility>
#include <iostream>
#include <random>

PositionalGenerator::PositionalGenerator(PositionalModelSerializer positionalModelSerializer)
        : positionalModelSerializer(std::move(positionalModelSerializer)) {}

void PositionalGenerator::generateTextInteractively(int generationSize) {

    std::map<std::string, std::vector<int>> model = positionalModelSerializer.getModel();
    std::vector<char> fileCache;

    RandomAccessReader randomAccessReader = RandomAccessReader(positionalModelSerializer.getInputFilePath());
    randomAccessReader.openFile();

    std::string input = std::string("");

    while (true) {

        std::cout << "Enter a sequence: ";
        std::getline(std::cin, input);

        if (input == "exit") break;

        if (input.size() < 3) {
            std::cout << "[!] Please enter a sequence with at least " << getModelWindowSize() << " characters." << std::endl;
            continue;
        }

        std::string currentWindow = input.substr(input.size() - 3, input.size());

        if (model.count(currentWindow) <= 0) {
            std::cout << "[!] Unable to find data for '" << input << "' in the model." << std::endl;
            std::cout << "[!] Please try again with a different sequence." << std::endl;
            continue;
        }

        std::cout << "[-] (" << input << ")";

        std::string completedOutput = currentWindow;

        while (completedOutput.size() < generationSize) {

            currentWindow = completedOutput.substr(completedOutput.size() - getModelWindowSize(), completedOutput.size());

            if (model.count(currentWindow) <= 0)
                break;

            char nextChar = generateNextCharacter(currentWindow, randomAccessReader);
            completedOutput += nextChar;

        }

        std::cout << completedOutput.substr(input.size(), completedOutput.size()) << std::endl;
        std::cout << std::endl;

    }

}

int PositionalGenerator::getModelWindowSize() {
    return (int) positionalModelSerializer.getModel().begin()->first.size();
}

char PositionalGenerator::generateNextCharacter(const std::string &currentWindow, RandomAccessReader randomAccessReader) {

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> uniformIntDistribution(1, getModelWindowSize()-1);

    unsigned int randomIndex = uniformIntDistribution(rng);
    return randomAccessReader.getCharAt(positionalModelSerializer.getModel()[currentWindow][randomIndex]);

}
