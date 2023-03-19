#include <iostream>
#include <random>
#include "input/classes/GeneratorInputArguments.h"
#include "input/InputUtils.h"
#include "modelSerializer/ModelSerializer.h"

char getNextCharacterUniform(const std::map<char, double>& map);
char getNextCharacterCustom(const std::map<char, double>& charactersProbabilityDistributionMap);
char getNextCharacterBacktracking(const std::string& currentWindow, std::map<std::string, std::map<char, double>> model);

int main(int argc, char *argv[]) {

    GeneratorInputArguments generatorInputArguments = getGeneratorInputArguments(argc, argv);

    cout << "Model File Path: " << generatorInputArguments.getModelPath() << endl;
    cout << "Amount of Characters to Generate: " << generatorInputArguments.getAmountOfCharactersToGenerate() << endl;

    ModelSerializer copyModelSerializer = ModelSerializer();
    copyModelSerializer.loadModel(generatorInputArguments.getModelPath());

    std::string input;

    cout << "-=-=-==-=-=-=-=-==-=-=-[ GENERATOR ]-=-=-==-=-=-=-=-==-=-=-" << endl;
    cout << endl;
    cout << "Write a sentence and the generator will try to complete it." << endl;
    cout << "Generating at most more " << generatorInputArguments.getAmountOfCharactersToGenerate() << " characters." << endl;
    cout << endl;
    cout << "When you are done, type 'exit'." << endl << endl;

    std::map<std::string, std::map<char, double>> model = copyModelSerializer.getModel();

    std::vector<char> generatedCharacters;

    do {

        cout << "Enter a sequence: ";
        std::getline(std::cin, input);

        if (input.size() < 3) {
            cout << "[!] Please enter a sequence with at least 3 characters." << endl;
            continue;
        }

        std::string currentWindow = input.substr(input.size()-3, input.size());

        if (model.count(currentWindow) <= 0) {
            cout << "[!] Unable to find data for '" << input << "' in the model." << endl;
            cout << "[!] Please try again with a different sequence." << endl;
            continue;
        }

        cout << "[-] ";
        cout << "(" << input << ")";

        for (int i = 0; i < generatorInputArguments.getAmountOfCharactersToGenerate(); ++i) {

            if (model.count(currentWindow) <= 0)
                break;

            char nextChar = getNextCharacterBacktracking(currentWindow, model);

            currentWindow += nextChar;
            currentWindow = currentWindow.substr(1, currentWindow.length() - 1);

            cout << nextChar;

        }

        cout << endl;

    } while (input != "exit");

    return 0;

}

bool sort_by_value(const pair<char, double>& a, const pair<char, double>& b) {
    return a.second > b.second;
}

char getNextCharacterBacktracking(const std::string& currentWindow, std::map<std::string, std::map<char, double>> model) {

    std::map<char, double> charactersProbabilityDistributionMap = model[currentWindow];
    vector<pair<char, double>> charactersProbabilityDistributionOrdered(charactersProbabilityDistributionMap.begin(),
                                                                        charactersProbabilityDistributionMap.end());

    std::sort(charactersProbabilityDistributionOrdered.begin(), charactersProbabilityDistributionOrdered.end(),
              sort_by_value);

    int max_tries = 1000;

    std::string nextWindow;
    int currentIndex = 0;
    int tries_counter = 0;
    char nextCharacterPrediction;

    do {

        // nextCharacterPrediction = charactersProbabilityDistributionOrdered[currentIndex++].first;
        nextCharacterPrediction = getNextCharacterUniform(charactersProbabilityDistributionMap);
        // nextCharacterPrediction = getNextCharacterCustom(charactersProbabilityDistributionMap);

        nextWindow = currentWindow.substr(1, currentWindow.size()-1) + nextCharacterPrediction;

        if (++tries_counter == max_tries) {
            nextCharacterPrediction = charactersProbabilityDistributionOrdered[0].first;
        }

    } while (model.count(nextWindow) <= 0 && tries_counter < max_tries);

    return nextCharacterPrediction;

}

char getNextCharacterUniform(const std::map<char, double>& map) {

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    double random = dist(mt);
    double sum = 0;

    for (const auto& [character, probability] : map) {
        sum += probability;
        if (random < sum)
            return character;
    }

    return map.begin()->first;

}


char getNextCharacterCustom(const std::map<char, double>& charactersProbabilityDistributionMap) {

    vector<pair<char, double>> charactersProbabilityDistributionOrdered(charactersProbabilityDistributionMap.begin(),
                                                                        charactersProbabilityDistributionMap.end());

    std::sort(charactersProbabilityDistributionOrdered.begin(), charactersProbabilityDistributionOrdered.end(), sort_by_value);

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> percentageDistribution(0.0, 1.0);

    double randomPercentage = percentageDistribution(mt);

    if (randomPercentage <= 0.70) {
        return charactersProbabilityDistributionOrdered[0].first;
    } else if (randomPercentage <= 0.85) {
        std::uniform_int_distribution<int> character(1, 4);
        int random = character(mt);
        return charactersProbabilityDistributionOrdered[random].first;
    } else if (randomPercentage <= 0.95) {
        std::uniform_int_distribution<int> character(5, 9);
        int random = character(mt);
        return charactersProbabilityDistributionOrdered[random].first;
    } else {
        std::uniform_int_distribution<int> character(10, (int) charactersProbabilityDistributionOrdered.size());
        int random = character(mt);
        return charactersProbabilityDistributionOrdered[random].first;
    }

}