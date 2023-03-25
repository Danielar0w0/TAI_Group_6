#include "FixedWindowModelBuilder.h"

void FixedWindowModelBuilder::buildModel(double alpha, double threshold) {

    fileReader.openFile();

    while (fileReader.next()) {

        std::string windowAsString = convertCharArrayToString(fileReader.getWindowContent(),
                                                                fileReader.getWindowSize());

        // We have seen this, let's try to predict
        if (pastSequencesPositions.count(windowAsString) > 0) {

            // Have we already selected a pointer for this sequence??
            if (sequenceCurrentPointer.count(windowAsString) <= 0) { // We have not selected a pointer for the current sequence. Select one (the first one)!!
                sequenceCurrentPointer[windowAsString] = pastSequencesPositions[windowAsString].rbegin()[0];
                sequenceCurrentPointerIndex[windowAsString] = 0;
            }

            // Now we have selected a pointer for this sequence for sure.
            int currentPointerForCurrentSequence = sequenceCurrentPointer[windowAsString];

            // Which character are we predicting that will come next?
            char predictedCharacter = fileReader.getCache()->operator[](currentPointerForCurrentSequence);

            // Which character was indeed next?
            char futureCharacter = fileReader.getNextCharacterInSequence();

            HitsMissesInfo hitsMissesInfo = HitsMissesInfo();

            // If we have information about Hits and Misses for this sequence then get it
            if (hitsAndMissesForEachSequence.count(windowAsString) > 0 && hitsAndMissesForEachSequence[windowAsString].first == predictedCharacter) {
                hitsMissesInfo = hitsAndMissesForEachSequence[windowAsString].second;
            }

            double probabilityForFutureCharacter = calculateHitProbability(hitsMissesInfo.getHits(),
                                                                           hitsMissesInfo.getMisses(), alpha);

            // Update probabilities for future character
            probabilitiesByCharacter[futureCharacter] += probabilityForFutureCharacter;

            // Keep track of symbol probabilities for current sequence
            sequenceSymbolProbabilitiesSum[windowAsString][futureCharacter] += probabilityForFutureCharacter;

            // Here we do a uniform distribution of probabilities across the different characters of the alphabet
            double probabilityDistribution = (1-probabilityForFutureCharacter)/(int)(fileInfo.getAlphabet().size()-1);

            // And assign the calculated probability to each one of the characters
            for (char characterInAlphabet : fileInfo.getAlphabet()) {

                if (characterInAlphabet != futureCharacter) {
                    probabilitiesByCharacter[characterInAlphabet] += probabilityDistribution;
                    // Keep track of symbol probabilities for current sequence
                    sequenceSymbolProbabilitiesSum[windowAsString][characterInAlphabet] += probabilityDistribution;
                }
            }

            // Update count to later calculate average of probabilities
            sequenceSymbolProbabilitiesCount[windowAsString]++;

            // Verify if it was a hit or a miss
            if (predictedCharacter == futureCharacter) { // Then we have a hit!
                hitsMissesInfo.incrementHits();
            } else {
                hitsMissesInfo.incrementMisses();
            }

            // hitsAndMissesForEachSequence[sequenceAsString].second = hitsMissesInfo; // Re-assign it
            hitsAndMissesForEachSequence[windowAsString] = std::make_pair(predictedCharacter, hitsMissesInfo);

            // Add this position to the vector of positions for this sequence
            pastSequencesPositions[windowAsString].push_back(fileReader.getCurrentPosition());

            // If our probability is below our threshold we need to change the pointer for our sequence because the current pointer is bad!
            // HOWEVER, we can only change pointer in we have another pointer to change to (check if size is greater than 1).
            if (probabilityForFutureCharacter < threshold && pastSequencesPositions[windowAsString].size() > 1) {

                // Change the pointer to another one
                int nextPointerIndex = ++sequenceCurrentPointerIndex[windowAsString];
                sequenceCurrentPointer[windowAsString] = pastSequencesPositions[windowAsString].rbegin()[nextPointerIndex];

                // Clean hits and misses for this sequence and this prediction
                hitsAndMissesForEachSequence[windowAsString] = std::make_pair(predictedCharacter, HitsMissesInfo());

            }

            ++totalAmountOfPredictions;

        } else {

            // If we have never seen this sequence, add it as key and a vector with the position
            pastSequencesPositions.insert(
                    std::pair<std::string, std::vector<int>>(windowAsString, {fileReader.getCurrentPosition()}));

        }

        std::printf("Progress (Fixed Window Model): %d%%\r", getProgress());
        std::fflush(stdout);

    }

    fileReader.closeFile();

}

std::map<std::string, std::map<char, double>> FixedWindowModelBuilder::getModel() {

    std::map<std::string, std::map<char, double>> sequenceSymbolProbabilities;

    //  Calculate probabilities for the model to be used in the generator
    for (const auto& i: sequenceSymbolProbabilitiesSum) {
        std::string sequence = i.first;
        for (auto j: i.second) {

            char c = j.first; double prob = j.second;

            // Update final average of probabilities for sequence + symbol
            int count = sequenceSymbolProbabilitiesCount[sequence];

            std::cout << prob << std::endl;
            std::cout << count << std::endl;

            sequenceSymbolProbabilities[sequence][c] = prob/count;

        }
    }

    return sequenceSymbolProbabilities;

}

double FixedWindowModelBuilder::calculateTotalInformation() {

    double totalInformation = 0;

    for (auto i: this->fileInfo.getSymbolsCount()) {
        char c = i.first;
        int count = i.second;
        totalInformation += count * -std::log2(probabilitiesByCharacter[c] / totalAmountOfPredictions);
    }

    return totalInformation;

}

double FixedWindowModelBuilder::calculateInformationByCharacter() {

//    std::map<char, double> informationByCharacter;
//
//    for (auto i : probabilitiesByCharacter) {
//        informationByCharacter[i.first] = -log2(i.second / totalAmountOfPredictions);
//    }

    return 0;

}

FixedWindowModelBuilder::FixedWindowModelBuilder(const FileReader &fileReader, const FileInfo &fileInfo)
        : AbstractModelBuilder(fileReader, fileInfo) {}

FixedWindowModelBuilder::~FixedWindowModelBuilder() = default;


