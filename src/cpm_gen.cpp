#include <iostream>

#include "input/implementation/GeneratorInputArguments.h"
#include "serialization/handler/ModelSerializerHandler.h"
#include "serialization/implementation/ProbabilisticModelSerializer.h"
#include "serialization/implementation/PositionalModelSerializer.h"
#include "reader/RandomAccessReader.h"
#include "generator/implementation/ProbabilisticGenerator.h"
#include "generator/implementation/PositionalGenerator.h"

int main(int argc, char *argv[]) {

    GeneratorInputArguments inputArguments = GeneratorInputArguments();
    inputArguments.parseArguments(argc, argv);

    Logger logger = Logger();
    logger.setLevel(inputArguments.getLoggingLevel());

    if (!inputArguments.checkArguments()) {
        GeneratorInputArguments::printUsage();
        return EXIT_FAILURE;
    }

    logger.info("Model File Path: " + inputArguments.getModelPath());
    logger.info("Amount of Characters to Generate: " + std::to_string(inputArguments.getAmountOfCharactersToGenerate()));

    ModelType modelType = ModelSerializerHandler::getModelType(inputArguments.getModelPath());

    if (modelType == ModelType::POSITIONAL) {

        PositionalModelSerializer sequentialModelSerializer = PositionalModelSerializer(inputArguments.getModelPath());
        sequentialModelSerializer.loadModel();

        PositionalGenerator positionalGenerator = PositionalGenerator(sequentialModelSerializer);
        positionalGenerator.printHeader();

        logger.info("[!] Using Positional Model");

        if (inputArguments.isInteractive()) {
            logger.info("[!] Using Interactive Mode");
            positionalGenerator.generateTextInteractively(inputArguments.getAmountOfCharactersToGenerate());
        } else {
            logger.info("[!] Using Non-Interactive Mode");
            positionalGenerator.generateTextOnce(inputArguments.getAmountOfCharactersToGenerate());
        }

    } else if (modelType == ModelType::PROBABILISTIC) {

        ProbabilisticModelSerializer probabilisticModelSerializer = ProbabilisticModelSerializer(inputArguments.getModelPath());
        probabilisticModelSerializer.loadModel();

        ProbabilisticGenerator probabilisticGenerator = ProbabilisticGenerator(probabilisticModelSerializer);
        probabilisticGenerator.printHeader();

        logger.info("[!] Using Probabilistic Model");

        if (inputArguments.shouldUseOptimization()) {
            logger.info("[!] Using Optimization");
            probabilisticGenerator.setUseOptimization(true);
        } else {
            logger.info("[!] Not Using Optimization");
            probabilisticGenerator.setUseOptimization(false);
        }

        if (inputArguments.isInteractive()) {
            logger.info("[!] Using Interactive Mode");
            probabilisticGenerator.generateTextInteractively(inputArguments.getAmountOfCharactersToGenerate());
        } else {
            logger.info("[!] Using Non-Interactive Mode");
            probabilisticGenerator.generateTextOnce(inputArguments.getAmountOfCharactersToGenerate());
        }

    } else {
        std::cout << "Model Type: Unknown" << std::endl;
    }

    return 0;

}