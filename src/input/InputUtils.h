#ifndef SRC_INPUTUTILS_H
#define SRC_INPUTUTILS_H

#include <iostream>
#include <regex>

#include "classes/InputArguments.h"
#include "../utils/printUtils.h"
#include "../utils/utils.h"

using namespace std;

InputArguments getInputArguments(int argc, char *argv[]) {

    cout << "You have entered " << argc - 1 << " arguments." << endl;

    if (argc == 1) {
        cerr << "No arguments were entered." << endl;
        printHelp();

        exit(EXIT_FAILURE);

    } else {

        int alpha = 0, k = 0; string filePath;

        // Note: argc - 1 because the last argument can't be a flag
        for (int i = 0; i < argc - 1; ++i) {

            // Handle alpha
            if (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--alpha") == 0) {

                if (isNumber(argv[i + 1]))
                    alpha = atoi(argv[i + 1]);


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
            alpha = 1;
        }

        if (k == 0) {
            cout << "Window size was not provided. Using default value: 3" << endl;
            k = 3;
        }

        return {filePath, alpha, k};

    }

}

#endif //SRC_INPUTUTILS_H
