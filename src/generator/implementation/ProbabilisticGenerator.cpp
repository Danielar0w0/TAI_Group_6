#include <iostream>
#include <utility>
#include <random>
#include <chrono>
#include "ProbabilisticGenerator.h"

ProbabilisticGenerator::ProbabilisticGenerator(ProbabilisticModelSerializer probabilisticModelSerializer)
        : probabilisticModelSerializer(std::move(probabilisticModelSerializer)) { }

void ProbabilisticGenerator::generateTextInteractively(int generationSize) {

    std::string input;
    std::map<std::string, std::map<char, double>> model = probabilisticModelSerializer.getModel();

    while (true) {

        std::cout << "Enter a sequence: ";
        std::getline(std::cin, input);

        if (input == "exit") break;

        std::cout << "[-] (" << input << ")";
        generateText(generationSize, input, model, true);

    }

}


void ProbabilisticGenerator::generateTextOnce(int generationSize) {

    std::string input;
    std::map<std::string, std::map<char, double>> model = probabilisticModelSerializer.getModel();

    std::cout << "Enter a sequence: ";
    std::getline(std::cin, input);

    if (input == "exit") return;

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    std::string generatedText = generateText(generationSize, input, model, false);

    if (generatedText.empty()) return;

    std::cout << "[-] (" << input << ")";
    std::cout << generatedText << std::endl;

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    logger.debug("Time taken to generate text: " + std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()) + "ms");

}

std::string ProbabilisticGenerator::generateText(int generationSize, const std::string &initialText,
                                                 const std::map<std::string, std::map<char, double>> &model,
                                                 bool realTime) {

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

    std::string generatedText = currentWindow;

    for (int i = 0; i < generationSize; ++i) {

        if (model.count(currentWindow) <= 0)
            break;

        char nextChar = generateNextCharacter(generatedText);

        generatedText += nextChar;

        if (realTime) {
            std::cout << nextChar;
            std::fflush(stdout);
        }


        currentWindow = generatedText.substr(generatedText.size()-getModelWindowSize(), generatedText.size());

    }

    if (realTime) {
        std::cout << std::endl;
        std::fflush(stdout);
    }

    return generatedText.substr(getModelWindowSize(), generatedText.length());

}

char ProbabilisticGenerator::generateNextCharacter(const std::string& generatedText) {

    std::string currentWindow = generatedText.substr(generatedText.size()-getModelWindowSize(), generatedText.size());
    std::map<std::string, std::map<char, double>> model = probabilisticModelSerializer.getModel();
    std::map<char, double> charactersProbabilityDistributionMap = model[currentWindow];

    return getCharacterUsingUniformDist(charactersProbabilityDistributionMap);

}

char ProbabilisticGenerator::getCharacterUsingUniformDist(const std::map<char, double>& charactersProbabilityDistribution) {

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    double random = dist(mt);
    double sum = 0;

    for (const auto& [character, probability] : charactersProbabilityDistribution) {
        sum += probability;
        if (random < sum)
            return character;
    }

    return charactersProbabilityDistribution.begin()->first;

}

int ProbabilisticGenerator::getModelWindowSize() {
    return (int) probabilisticModelSerializer.getModel().begin()->first.size();
}
