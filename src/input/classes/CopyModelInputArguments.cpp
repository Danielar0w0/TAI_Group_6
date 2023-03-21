#include "CopyModelInputArguments.h"

CopyModelInputArguments::CopyModelInputArguments(std::string inputFilePath, std::string outputFilePath, double alpha,
                                                 int k, int modelBuilderType) {
    this->alpha = alpha;
    this->k = k;
    this->inputFilePath = std::move(inputFilePath);
    this->outputModelPath = std::move(outputFilePath);
    this->modelBuilderType = modelBuilderType;

}

double CopyModelInputArguments::getAlpha() const {
    return this->alpha;
}

int CopyModelInputArguments::getK() const {
    return this->k;
}

double CopyModelInputArguments::getThreshold() const {
    return this->threshold;
}

std::string CopyModelInputArguments::getInputFilePath() const {
    return this->inputFilePath;
}

bool CopyModelInputArguments::checkArguments() const {

    if (this->alpha <= 0 || this->alpha > 5) {
        std::cerr << "[!!!] Alpha should be a value in the interval ]0, 5]." << std::endl;
        return false;
    }

    if (this->k <= 0 || this->k > 100) {
        std::cerr << "[!!!] K should be a value in the interval ]0, 100]." << std::endl;
        return false;
    }

    if (this->inputFilePath.empty()) {
        std::cerr << "[!!!] The target file was not provided." << std::endl;
        return false;
    }

    if (this->outputModelPath.empty()) {
        std::cerr << "[!!!] The output file for the model was not provided." << std::endl;
        return false;
    }

    if (this->modelBuilderType != 1 && this->modelBuilderType != 2) {
        std::cerr << "[!!!] The model builder type should be 1 or 2." << std::endl;
        return false;
    }

    if (this->threshold <= 0 || this->threshold > 1) {
        std::cerr << "[!!!] The threshold should be a value in the interval ]0, 1]." << std::endl;
        return false;
    }

    return true;

}

void CopyModelInputArguments::printUsage() {
    std::cout << "cmp: cmp [-akf]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "-a \t Alpha" << std::endl;
    std::cout << "-k \t Window size" << std::endl;
    std::cout << "-t \t Threshold" << std::endl;
    std::cout << "-i \t File with target sequence" << std::endl;
    std::cout << "-o \t File to save the model" << std::endl;
    std::cout << "-m \t Model builder type" << std::endl;
}

std::string CopyModelInputArguments::getOutputModelPath() const {
    return this->outputModelPath;
}

int CopyModelInputArguments::getModelBuilderType() const {
    return this->modelBuilderType;
}

void CopyModelInputArguments::parseArguments(int argc, char **argv) {

    if (argc == 1) {
        std::cerr << "No arguments were entered." << std::endl;
        CopyModelInputArguments::printUsage();
        exit(EXIT_FAILURE);
    } else {

        int parsedK = 0;
        double parsedAlpha = 0;
        double parsedThreshold = 0;
        std::string parsedInputFilePath;
        std::string parsedOutputModelFilePath;
        int parsedModelBuilderType = -1;

        // Note: argc - 1 because the last argument can't be a flag
        for (int i = 0; i < argc - 1; ++i) {

            // Handle parsedAlpha
            if (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--alpha") == 0) {
                if (isNumber(argv[i + 1]))
                    parsedAlpha = atof(argv[i + 1]);
            } else if (strcmp(argv[i], "-k") == 0) {
                if (isNumber(argv[i + 1]))
                    parsedK = atoi(argv[i + 1]);
            } else if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--input") == 0) {
                parsedInputFilePath = argv[i + 1];
            } else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0) {
                parsedOutputModelFilePath = argv[i + 1];
            } else if (strcmp(argv[i], "-m") == 0 || strcmp(argv[i], "--model") == 0) {
                if (isNumber(argv[i + 1]))
                    parsedModelBuilderType = atoi(argv[i + 1]);
            } else if (strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--threshold") == 0) {
                if (isNumber(argv[i + 1]))
                    parsedThreshold = atof(argv[i + 1]);
            }
        }

        if (parsedAlpha == 0) {
            std::cout << "Alpha was not provided. Using default value: 1" << std::endl;
            parsedAlpha = 1;
        }

        if (parsedK == 0) {
            std::cout << "Window size was not provided. Using default value: 3" << std::endl;
            parsedK = 3;
        }

        if (parsedThreshold == 0) {
            std::cout << "Threshold was not provided. Using default value: 0.4" << std::endl;
            parsedThreshold = 0.4;
        }

        if (parsedModelBuilderType == -1) {
            std::cout << "Model builder type was not provided. Using default value: 1" << std::endl;
            parsedModelBuilderType = 1;
        }

        this->alpha = parsedAlpha;
        this->k = parsedK;
        this->threshold = parsedThreshold;
        this->inputFilePath = std::move(parsedInputFilePath);
        this->outputModelPath = std::move(parsedOutputModelFilePath);
        this->modelBuilderType = parsedModelBuilderType;

    }

}

CopyModelInputArguments::CopyModelInputArguments() = default;
