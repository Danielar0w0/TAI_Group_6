#include "FixedWindowModelBuilder.h"

void FixedWindowModelBuilder::buildModel(double alpha, double threshold) {

    fileReader.openFile();

    while (fileReader.next()) {

        std::string windowAsString = convertCharArrayToString(fileReader.getWindowContent(),
                                                                fileReader.getWindowSize());

        // If we have never seen this sequence, add it as key and a vector with the position
        pastSequencesPositions[windowAsString].push_back(fileReader.getCurrentPosition());

        // Which character is next?
        char futureCharacter = fileReader.getNextCharacterInSequence();

        probabilitiesForSequence[windowAsString][futureCharacter] += 1;

        std::printf("Progress (Fixed Window Model): %d%%\r", getProgress());
        std::fflush(stdout);

    }

    // Calculate the probabilities
    for (auto &sequence : probabilitiesForSequence) {
        for (auto &character : sequence.second) {
            character.second /= (double) pastSequencesPositions[sequence.first].size();
        }
    }

    fileReader.closeFile();

}

std::map<std::string, std::map<char, double>> FixedWindowModelBuilder::getProbabilisticModel() {
    return probabilitiesForSequence;
}

std::map<std::string, std::vector<int>> FixedWindowModelBuilder::getPositionModel() {
    return pastSequencesPositions;
}

FixedWindowModelBuilder::FixedWindowModelBuilder(const FileReader &fileReader, const FileInfo &fileInfo,
                                                 const Logger &logger) : AbstractModelBuilder(fileReader, fileInfo,
                                                                                              logger) {}

FixedWindowModelBuilder::~FixedWindowModelBuilder() = default;


