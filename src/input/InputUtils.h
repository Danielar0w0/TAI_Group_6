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

    cout << "You have entered " << argc - 1 << " arguments." << endl;

    if (argc == 1) {
        cerr << "No arguments were entered." << endl;
        CopyModelInputArguments::printUsage();
        exit(EXIT_FAILURE);
    } else {

        int k = 0;
        double alpha = 0;
        string filePath;

        // Note: argc - 1 because the last argument can't be a flag
        for (int i = 0; i < argc - 1; ++i) {

            // Handle alpha
            if (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--alpha") == 0) {

                if (isNumber(argv[i + 1]))
                    alpha = atof(argv[i + 1]);

            // Handle window size
            } else if (strcmp(argv[i], "-k") == 0) {
                if (isNumber(argv[i + 1]))
                    k = atoi(argv[i + 1]);

            // Handle file path (target sequence)
            } else if (strcmp(argv[i], "-f") == 0) {
                filePath = argv[i + 1];
            }
        }

        if (alpha == 0) {
            cout << "Alpha was not provided. Using default value: 2" << endl;
            alpha = 2;
        }

        if (k == 0) {
            cout << "Window size was not provided. Using default value: 3" << endl;
            k = 3;
        }

        return {filePath, alpha, k};

    }

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
