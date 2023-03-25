#include <iostream>
#include <cmath>

#include "utils/printUtils.h"
#include "input/classes/CopyModelInputArguments.h"
#include "reader/FileReader.h"
#include "model_builder/utils/HitsMissesInfo.h"
#include "model_serialization/implementation/ProbabilisticModelSerializer.h"
#include "model_serialization/implementation/SequentialModelSerializer.h"
#include "model_builder/implementation/FixedWindowModelBuilder.h"
#include "model_builder/implementation/GrowingWindowModelBuilder.h"


FileInfo getFileInfo(const CopyModelInputArguments& inputArguments);
FileReader getFileReaderInstance(const CopyModelInputArguments& inputArguments);
void runModelBuilder(const CopyModelInputArguments& inputArguments, const FileInfo& fileInfo, const FileReader& fileReader);
void printInformationForCharacters(const std::map<char, double>& informationForCharacters);

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

    std::cout << "Alphabet: " << fileInfo.getSize() << std::endl;

    // Get File Reader instance for second pass
    FileReader fileReader = getFileReaderInstance(inputArguments);

    if (inputArguments.getModelBuilderType() == 1) {
        std::cout << "Running Growing Window Model Builder..." << std::endl;
    } else if (inputArguments.getModelBuilderType() == 2) {
        std::cout << "Running Fixed Window Model Builder..." << std::endl;
    } else {
        std::cout << "Invalid Model Builder Type. Exiting..." << std::endl;
        std::exit(EXIT_FAILURE);
    }

    runModelBuilder(inputArguments, fileInfo, fileReader);

    return 0;

}

void runModelBuilder(const CopyModelInputArguments& inputArguments, const FileInfo& fileInfo, const FileReader& fileReader) {

    if (inputArguments.getModelBuilderType() == 1) {

        GrowingWindowModelBuilder growingWindowModelBuilder = GrowingWindowModelBuilder(fileReader, fileInfo);
        growingWindowModelBuilder.buildModel(inputArguments.getAlpha(), inputArguments.getThreshold());

        SequentialModelSerializer sequentialModelSerializer = SequentialModelSerializer(inputArguments.getOutputModelPath());
        sequentialModelSerializer.setModel(growingWindowModelBuilder.getModel());
        sequentialModelSerializer.outputModel();

        std::cout << "Information by Character: " << growingWindowModelBuilder.calculateInformationByCharacter() << std::endl;
        std::cout << "Total Amount of Information: " << growingWindowModelBuilder.calculateTotalInformation() << std::endl;

    } else if (inputArguments.getModelBuilderType() == 2) {

        FixedWindowModelBuilder fixedWindowModelBuilder = FixedWindowModelBuilder(fileReader, fileInfo);
        fixedWindowModelBuilder.buildModel(inputArguments.getAlpha(), inputArguments.getThreshold());

        ProbabilisticModelSerializer probabilisticModelSerializer = ProbabilisticModelSerializer(inputArguments.getOutputModelPath());
        probabilisticModelSerializer.setModel(fixedWindowModelBuilder.getModel());
        probabilisticModelSerializer.outputModel();

        std::cout << "Information by Character: " << fixedWindowModelBuilder.calculateInformationByCharacter() << std::endl;
        std::cout << "Total Amount of Information: " << fixedWindowModelBuilder.calculateTotalInformation() << std::endl;

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


