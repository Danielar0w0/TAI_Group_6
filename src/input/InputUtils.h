#ifndef SRC_INPUTUTILS_H
#define SRC_INPUTUTILS_H

#include <iostream>
#include <regex>

#include "classes/CopyModelInputArguments.h"
#include "../utils/printUtils.h"
#include "../utils/generalUtils.h"
#include "classes/GeneratorInputArguments.h"

using namespace std;

CopyModelInputArguments getCopyModelInputArguments(int argc, char *argv[]) {



}

GeneratorInputArguments getGeneratorInputArguments(int argc, char *argv[]) {

    cout << "You have entered " << argc - 1 << " arguments." << endl;

    if (argc == 1) {
        cerr << "No arguments were entered." << endl;
        GeneratorInputArguments::printUsage();
        exit(EXIT_FAILURE);
    } else {

        int amountOfCharactersToGenerate = 0;
        string modelFilePath;

        // Note: argc - 1 because the last argument can't be a flag
        for (int i = 0; i < argc - 1; ++i) {
            if (strcmp(argv[i], "-m") == 0 || strcmp(argv[i], "--model") == 0) {
                    modelFilePath = argv[i + 1];
            } else if (strcmp(argv[i], "-g") == 0 || strcmp(argv[i], "--generationCharacters") == 0) {
                if (isNumber(argv[i + 1]))
                    amountOfCharactersToGenerate = atoi(argv[i + 1]);
            }
        }

        if (modelFilePath.empty()) {
            cerr << "The Path of the model file was not provided." << endl;
            exit(EXIT_FAILURE);
        }

        if (amountOfCharactersToGenerate == 0) {
            cout << "Amount of Characters to generate not provided. Using default value: 10" << endl;
            amountOfCharactersToGenerate = 10;
        }

        return GeneratorInputArguments(modelFilePath, amountOfCharactersToGenerate);

    }

}

#endif
