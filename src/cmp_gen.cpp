#include <iostream>
#include <random>
#include "input/classes/GeneratorInputArguments.h"
#include "input/InputUtils.h"
#include "modelSerializer/ModelSerializer.h"

char getNextCharacter(const std::map<char, double>& map);

int main(int argc, char *argv[]) {

    GeneratorInputArguments generatorInputArguments = getGeneratorInputArguments(argc, argv);

    cout << "Model File Path: " << generatorInputArguments.getModelPath() << endl;
    cout << "Amount of Characters to Generate: " << generatorInputArguments.getAmountOfCharactersToGenerate() << endl;

    ModelSerializer copyModelSerializer = ModelSerializer();
    copyModelSerializer.loadModel(generatorInputArguments.getModelPath());

    std::string input;

    cout << "-=-=-==-=-=-=-=-==-=-=-[ GENERATOR ]-=-=-==-=-=-=-=-==-=-=-" << endl;
    cout << endl;
    cout << "Write a sentence with 3 characters MAX and the generator will try to complete the sentence with "
        << generatorInputArguments.getAmountOfCharactersToGenerate() << " more characters." << endl;
    cout << "When you are done, type 'exit'." << endl << endl;

    std::map<std::string, std::map<char, double>> model = copyModelSerializer.getModel();

    do {

        cout << "Enter a sequence: ";
        std::cin >> input;

        if (model.count(input) <= 0) {
            cout << "[!] Unable to find data for '" << input << "' in the model." << endl;
            cout << "[!] Please try again with a different sequence." << endl;
            continue;
        }

        std::string currentWindow = input;

        cout << "[-] ";
        cout << "(" << input << ")";

        for (int i = 0; i < generatorInputArguments.getAmountOfCharactersToGenerate(); ++i) {

            if (model.count(currentWindow) <= 0)
                break;

            char nextChar = getNextCharacter(model[currentWindow]);

            currentWindow += nextChar;
            currentWindow = currentWindow.substr(1, currentWindow.length() - 1);

            cout << nextChar;

        }

        cout << endl;

    } while (input != "exit");

    return 0;

}

char getNextCharacter(const std::map<char, double>& map) {

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    double random = dist(mt);
    double sum = 0;

    for (auto &it : map) {
        sum += it.second;
        if (random <= sum)
            return it.first;
    }

    return 0x00;

}