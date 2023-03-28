#include "PositionalGenerator.h"

#include <utility>
#include <iostream>
#include <random>
#include <chrono>

PositionalGenerator::PositionalGenerator(PositionalModelSerializer positionalModelSerializer)
        : positionalModelSerializer(std::move(positionalModelSerializer)) {}


std::string PositionalGenerator::generateText(int generationSize, const std::string& initialText, const std::map<std::string, std::vector<int>>& model, bool realTime) {

    RandomAccessReader randomAccessReader = RandomAccessReader(positionalModelSerializer.getInputFilePath());
    randomAccessReader.openFile();

    if (initialText.size() < getModelWindowSize()) {
        std::cout << "[!] Please enter a sequence with at least " << getModelWindowSize() << " characters." << std::endl;
        return "";
    }

    std::string currentWindow = initialText.substr(initialText.size()-getModelWindowSize(), initialText.size());

    if (model.count(currentWindow) <= 0) {
        std::cout << "[!] Unable to find data for '" << currentWindow << "' in the model." << std::endl;
        std::cout << "[!] Please try again with a different sequence." << std::endl;
        return "";
    }

    std::string completedOutput;

    while (completedOutput.size() < generationSize) {

        currentWindow = currentWindow.substr(currentWindow.size()-getModelWindowSize(), currentWindow.size());

        if (model.count(currentWindow) <= 0)
            break;

        char nextChar = generateNextCharacter(currentWindow, randomAccessReader);
        completedOutput += nextChar;
        currentWindow += nextChar;

        if (realTime) {
            std::cout << nextChar;
            std::fflush(stdout);
        }

    }

    if (realTime) {
        std::cout << std::endl;
        std::fflush(stdout);
    }

    return completedOutput;

}

void PositionalGenerator::generateTextInteractively(int generationSize) {

    std::map<std::string, std::vector<int>> model = positionalModelSerializer.getModel();
    std::string input;

    while (true) {

        std::cout << "Enter a sequence: ";
        std::getline(std::cin, input);

        if (input == "exit") break;

        std::cout << "[-] (" << input << ")";

        generateText(generationSize, input, model, true);

    }

}

void PositionalGenerator::generateTextOnce(int generationSize) {

    std::map<std::string, std::vector<int>> model = positionalModelSerializer.getModel();
    std::string input = std::string("");

    std::cout << "Enter a sequence: ";
    std::getline(std::cin, input);

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    std::string textGenerated = generateText(generationSize, input, model, false);

    if (textGenerated.empty()) return;

    std::cout << "[-] (" << input << ")";
    std::cout << textGenerated << std::endl;

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    logger.debug("Time taken to generate text: " + std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()) + "ms");

}

int PositionalGenerator::getModelWindowSize() {
    return (int) positionalModelSerializer.getModel().begin()->first.size();
}

char PositionalGenerator::generateNextCharacter(const std::string &currentWindow, RandomAccessReader randomAccessReader) {

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> uniformIntDistribution(0, positionalModelSerializer.getModel()[currentWindow].size()-1);

    unsigned int randomIndex = uniformIntDistribution(rng);

    return randomAccessReader.getCharAt(positionalModelSerializer.getModel()[currentWindow][randomIndex]);

}
