#include "ModelGenerator.h"

#include <random>

void ModelGenerator::generateModel() {

    fileReader.openFile();

    while (fileReader.next()) {

        std::string windowAsString = convertCharArrayToString(fileReader.getWindowContent(),
                                                                fileReader.getWindowSize());

        // If we have never seen this sequence, add it as key and a vector with the position
        if (std::find(pastSequencesPositions[windowAsString].begin(),
                         pastSequencesPositions[windowAsString].end(),fileReader.getCurrentPosition()+1) == pastSequencesPositions[windowAsString].end())
            pastSequencesPositions[windowAsString].push_back(fileReader.getCurrentPosition());

        // Which character is next?
        char futureCharacter = fileReader.getNextCharacterInSequence();

        probabilitiesForSequence[windowAsString][futureCharacter] += 1;

        std::printf("Model Generator Progress: %d%%\r", getProgress());
        std::fflush(stdout);

    }

    std::printf("\n");
    std::fflush(stdout);

    // Calculate the probabilities
    for (auto &sequence : probabilitiesForSequence) {

        for (auto &character : sequence.second) {
            character.second /= (double) pastSequencesPositions[sequence.first].size();
            character.second *= 0.9;
        }

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, (int)fileInfo.getAlphabet().size()-1);

        std::vector<char> alphabetVector;

        // Print all characters in alphabetVector
        for (auto &character : fileInfo.getAlphabet()) {
            alphabetVector.push_back(character);
        }

        for (int i = 0; i < 5; ++i) {

            char character = alphabetVector[dis(gen)];

            if (sequence.second.count(character) == 0) {
                sequence.second[character] = 0.1/5;
            }

        }

    }

    fileReader.closeFile();

}

std::map<std::string, std::map<char, double>> ModelGenerator::getProbabilisticModel() {
    return probabilitiesForSequence;
}

std::map<std::string, std::vector<int>> ModelGenerator::getPositionModel() {
    return pastSequencesPositions;
}

ModelGenerator::ModelGenerator(const FileReader &fileReader, const FileInfo &fileInfo)
        : InputProcessorEntity(
        fileReader, fileInfo) {}

ModelGenerator::~ModelGenerator() = default;


