#include <iostream>
#include <utility>
#include <random>
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

        std::string generatedText = generateText(generationSize, input, model, this->useOptimization);

        if (generatedText.empty()) continue;

        std::cout << "[-] (" << input << ")";
        std::cout << generatedText << std::endl;

    }

}


void ProbabilisticGenerator::generateTextOnce(int generationSize) {

    std::string input;
    std::map<std::string, std::map<char, double>> model = probabilisticModelSerializer.getModel();

    std::cout << "Enter a sequence: ";
    std::getline(std::cin, input);

    if (input == "exit") return;

    std::string generatedText = generateText(generationSize, input, model, this->useOptimization);

    if (generatedText.empty()) return;

    std::cout << "[-] (" << input << ")";
    std::cout << generatedText << std::endl;

}

std::string ProbabilisticGenerator::generateText(int generationSize, const std::string &initialText,
                                                 const std::map<std::string, std::map<char, double>> &model,
                                                 bool useFuturePredictionAlgorithm) {

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

        char nextChar = generateNextCharacter(generatedText, useFuturePredictionAlgorithm);

        currentWindow += nextChar;
        generatedText += nextChar;
        currentWindow = currentWindow.substr(1, currentWindow.length() - 1);

    }

    return generatedText.substr(getModelWindowSize(), generatedText.length());

}

char ProbabilisticGenerator::generateNextCharacter(const std::string& generatedText, bool useFuturePredictionAlgorithm) {

    std::string currentWindow = generatedText.substr(generatedText.size()-getModelWindowSize(), generatedText.size());
    std::map<std::string, std::map<char, double>> model = probabilisticModelSerializer.getModel();
    std::map<char, double> charactersProbabilityDistributionMap = model[currentWindow];

    if (useFuturePredictionAlgorithm) {

        int maximumThreshold = 100;
        int triesCounter = 0;
        std::string nextWindow;
        char nextCharacterPrediction;
        std::vector<char> notSafeCharacters;

        while (true) {

            char randomPrediction;

            if (notSafeCharacters.size() >= charactersProbabilityDistributionMap.size()/5) {

                for (const auto& [futureCharacter, _] : charactersProbabilityDistributionMap) {
                    if (std::find(notSafeCharacters.begin(), notSafeCharacters.end(), futureCharacter) == notSafeCharacters.end()) {
                        randomPrediction = futureCharacter;
                        break;
                    }
                }

            } else {
                randomPrediction = getCharacterUsingUniformDist(charactersProbabilityDistributionMap);
            }

            currentWindow = currentWindow.substr(1, currentWindow.size()-1) + randomPrediction;

            // If the window + the character I generated exists in the model
            if (model.count(currentWindow) > 0) {

                if (isCharacterSafeChoice(currentWindow, charactersProbabilityDistributionMap, 0, this->optimizationAggressiveness)) {
                    nextCharacterPrediction = randomPrediction;
                    notSafeCharacters.clear();
                    std::cout << "Optimizing Output. Optimizing " << generatedText.size() << "th character." << "\r";
                    std::fflush(stdout);
                    break;
                } else {
                    notSafeCharacters.push_back(randomPrediction);
                }

                continue;

            } else { // If the window + the character does not exist

                if (++triesCounter >= maximumThreshold) {
                    nextCharacterPrediction = randomPrediction;
                    break;
                }

                continue;

            }

        }

        return nextCharacterPrediction;

    } else {
        return getCharacterUsingUniformDist(charactersProbabilityDistributionMap);
    }

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

bool ProbabilisticGenerator::isCharacterSafeChoice(const std::string& currentWindow,  const std::map<char, double>& charactersProbabilityDistribution,
                                                   int currentIteration, int maxIterations) {
    int totalCorrectMoves = 0;
    std::string tempWindow;

    if (currentIteration >= maxIterations) {
        logger.debug("Reached max iterations: " + std::to_string(maxIterations));
        return true;
    }

    // Check if in the next move it will come to a point where I'm trapped
    for (const auto& [futureCharacter, _] : charactersProbabilityDistribution) {

        tempWindow = currentWindow.substr(1, currentWindow.size()-1) + futureCharacter;

        if (probabilisticModelSerializer.getModel().count(tempWindow) > 0) {
            totalCorrectMoves++;
        }

    }

    if (totalCorrectMoves >= this->optimizationAggressiveness/100*charactersProbabilityDistribution.size())
        return isCharacterSafeChoice(tempWindow, charactersProbabilityDistribution, currentIteration + 1,maxIterations);
    return false;

}

int ProbabilisticGenerator::getModelWindowSize() {
    return (int) probabilisticModelSerializer.getModel().begin()->first.size();
}

void ProbabilisticGenerator::setUseOptimization(bool useOptimizationParam) {
    this->useOptimization = useOptimizationParam;
}

void ProbabilisticGenerator::setOptimizationAggressiveness(int optimizationAggressivenessParam) {
    this->optimizationAggressiveness = optimizationAggressivenessParam;
}
