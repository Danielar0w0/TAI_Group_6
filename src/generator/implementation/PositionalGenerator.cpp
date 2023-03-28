#include "PositionalGenerator.h"

#include <utility>
#include <iostream>
#include <random>

PositionalGenerator::PositionalGenerator(PositionalModelSerializer positionalModelSerializer)
        : positionalModelSerializer(std::move(positionalModelSerializer)) {}


std::string PositionalGenerator::generateText(int generationSize, const std::string& initialText, const std::map<std::string, std::vector<int>>& model) {

    RandomAccessReader randomAccessReader = RandomAccessReader(positionalModelSerializer.getInputFilePath());
    randomAccessReader.openFile();

    if (initialText.size() < getModelWindowSize()) {
        std::cout << "[!] Please enter a sequence with at least " << getModelWindowSize() << " characters." << std::endl;
        return "";
    }

    std::string currentWindow = initialText.substr(initialText.size() - getModelWindowSize(), initialText.size());

    if (model.count(currentWindow) <= 0) {
        std::cout << "[!] Unable to find data for '" << currentWindow << "' in the model." << std::endl;
        std::cout << "[!] Please try again with a different sequence." << std::endl;
        return "";
    }

    std::string completedOutput = currentWindow;

    while (completedOutput.size() < generationSize) {

        currentWindow = completedOutput.substr(completedOutput.size()-getModelWindowSize(), completedOutput.size());

        if (model.count(currentWindow) <= 0)
            break;

        char nextChar = generateNextCharacter(currentWindow, randomAccessReader);
        completedOutput += nextChar;

    }

    return completedOutput.substr(initialText.size(), completedOutput.size());

}

void PositionalGenerator::generateTextInteractively(int generationSize) {

    std::map<std::string, std::vector<int>> model = positionalModelSerializer.getModel();
    std::string input = std::string("");

    while (true) {

        std::cout << "Enter a sequence: ";
        std::getline(std::cin, input);

        if (input == "exit") break;

        std::string textGenerated = generateText(generationSize, input, model);

        if (textGenerated.empty()) continue;

        std::cout << "[-] (" << input << ")";
        std::cout << textGenerated << std::endl;

    }

}

void PositionalGenerator::generateTextOnce(int generationSize) {

    std::map<std::string, std::vector<int>> model = positionalModelSerializer.getModel();
    std::string input = std::string("");

    std::cout << "Enter a sequence: ";
    std::getline(std::cin, input);

    std::string textGenerated = generateText(generationSize, input, model);

    if (textGenerated.empty()) return;

    std::cout << "[-] (" << input << ")";
    std::cout << textGenerated << std::endl;


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
