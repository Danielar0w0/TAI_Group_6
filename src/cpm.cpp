#include <cmath>

#include "utils/printUtils.h"
#include "input/classes/CopyModelInputArguments.h"
#include "reader/FileReader.h"
#include "cpm/builder/CopyModelBuilder.h"
#include "logging/Logger.h"
#include "cpm/generation/ModelGenerator.h"
#include "serialization/utils/ModelType.h"
#include "serialization/implementation/ProbabilisticModelSerializer.h"
#include "serialization/implementation/PositionalModelSerializer.h"


FileInfo getFileInfo(const CopyModelInputArguments& inputArguments);
FileReader getFileReaderInstance(const CopyModelInputArguments& inputArguments);

void logCopyModelResults(CopyModelBuilder* copyModelBuilder, Logger logger);
CopyModelBuilder* runModelBuilder(const CopyModelInputArguments& inputArguments, const FileInfo& fileInfo,
                     const FileReader& fileReader, Logger logger);

ModelGenerator* generateModel(const FileInfo& fileInfo, const FileReader& fileReader, Logger logger);
void serializeModel(ModelGenerator* modelGenerator, const std::string& modelPath, const std::string& inputFilePath,
                    int generatedModelType);


int main(int argc, char *argv[]) {

    // Read command line arguments
    CopyModelInputArguments inputArguments = CopyModelInputArguments();
    inputArguments.parseArguments(argc, argv);

    bool areArgumentsValid = inputArguments.checkArguments();

    if (!areArgumentsValid) {
        CopyModelInputArguments::printUsage();
        std::exit(EXIT_FAILURE);
    }

    // First Pass: Get File Info. (Alphabet and size)
    FileInfo fileInfo = getFileInfo(inputArguments);

    // Initialize logger
    Logger logger = Logger();
    logger.setLevel(inputArguments.getLoggingLevel());

    // Get File Reader instance for second pass
    FileReader fileReader = getFileReaderInstance(inputArguments);

    CopyModelBuilder* copyModelBuilder = runModelBuilder(inputArguments, fileInfo, fileReader, logger);

    logCopyModelResults(copyModelBuilder, logger);

    delete copyModelBuilder;

    ModelGenerator* modelGenerator = generateModel(fileInfo, fileReader, logger);

    serializeModel(modelGenerator, inputArguments.getOutputModelPath(),
                   inputArguments.getInputFilePath(),inputArguments.getSerializerType());

    return EXIT_SUCCESS;

}

CopyModelBuilder* runModelBuilder(const CopyModelInputArguments& inputArguments, const FileInfo& fileInfo,
                     const FileReader& fileReader, Logger logger) {

    auto* copyModelBuilder = new CopyModelBuilder(fileReader, fileInfo);
    copyModelBuilder->setLogger(logger);

    copyModelBuilder->buildModel(inputArguments.getAlpha(), inputArguments.getThreshold());

    return copyModelBuilder;

}

void logCopyModelResults(CopyModelBuilder* copyModelBuilder, Logger logger) {

    logger.info("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-");
    logger.info("Copy Model Results");
    logger.info("");
    logger.info("Analyzed File: " + copyModelBuilder->getFileInfo().getFilePath());
    logger.info("File Size (Characters): " + std::to_string(copyModelBuilder->getFileInfo().getSize()));

    std::string alphabetStr;
    for (char characterInAlphabet : copyModelBuilder->getFileInfo().getAlphabet()) {
        alphabetStr.append(std::string(1, characterInAlphabet));
        alphabetStr.append(",");
    }

    logger.info("Alphabet: " + alphabetStr);
    logger.info("");
    logger.info("Total Amount of Information: " + std::to_string(copyModelBuilder->calculateTotalInformation()));
    logger.info("Information p/ character: " + std::to_string(copyModelBuilder->calculateInformationByCharacter()));
    logger.info("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-");
    logger.info("\n");

}

ModelGenerator* generateModel(const FileInfo& fileInfo, const FileReader& fileReader, Logger logger) {

    auto* modelGenerator = new ModelGenerator(fileReader, fileInfo);
    modelGenerator->setLogger(logger);

    modelGenerator->generateModel();

    return modelGenerator;

}

void serializeModel(ModelGenerator* modelGenerator, const std::string& modelPath, const std::string& inputFilePath,
                    int generatedModelType) {

    if (generatedModelType == ModelType::PROBABILISTIC) {

        ProbabilisticModelSerializer probabilisticModelSerializer = ProbabilisticModelSerializer(modelPath);
        probabilisticModelSerializer.setInputFilePath(inputFilePath);
        probabilisticModelSerializer.setModel(modelGenerator->getProbabilisticModel());

        probabilisticModelSerializer.outputModel();

    } else if (generatedModelType == ModelType::POSITIONAL) {

        PositionalModelSerializer positionalModelSerializer = PositionalModelSerializer(modelPath);
        positionalModelSerializer.setInputFilePath(inputFilePath);
        positionalModelSerializer.setModel(modelGenerator->getPositionModel());

        positionalModelSerializer.outputModel();

    }

}

FileInfo getFileInfo(const CopyModelInputArguments& inputArguments) {

    FileReader fileReader = getFileReaderInstance(inputArguments);

    fileReader.openFile();
    FileInfo fileInfo = fileReader.getFileInfo();
    fileReader.closeFile();

    return fileInfo;
}

FileReader getFileReaderInstance(const CopyModelInputArguments& inputArguments) {
    return FileReader(inputArguments.getInputFilePath(), inputArguments.getK());
}


