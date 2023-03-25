#include <iostream>
#include <random>
#include "GrowingWindowModelBuilder.h"

GrowingWindowModelBuilder::GrowingWindowModelBuilder(const FileReader &fileReader, const FileInfo &fileInfo) : AbstractModelBuilder(fileReader, fileInfo) {}

GrowingWindowModelBuilder::~GrowingWindowModelBuilder() = default;

void GrowingWindowModelBuilder::buildModel(double alpha, double threshold) {

    fileReader.openFile();

    // Read a window
    while (fileReader.next()) {

        // Get Window sequence as string
        std::string sequenceAsString = convertCharArrayToString(fileReader.getWindowContent(), fileReader.getWindowSize());

        // We have seen this sequence in the past
        if (pastSequencesPositions.count(sequenceAsString) > 0) {

            int currentPointerIndex;

            // If I haven't chosen a pointer for this sequence yet, chose the first pointer
            if (currentPointerIndexForSequence.count(sequenceAsString) <= 0) {
                currentPointerIndex = 0;
                currentPointerIndexForSequence.insert(std::make_pair(sequenceAsString, currentPointerIndex));
            } else {
                currentPointerIndex = currentPointerIndexForSequence[sequenceAsString];
            }

            // If we saw this sequence before, just add this new position
            pastSequencesPositions[sequenceAsString].push_back(fileReader.getCurrentPosition());

            int pastSequencePosition = pastSequencesPositions[sequenceAsString].rbegin()[currentPointerIndex];

            // The probability P (probability of the next character being equal to the one I'm seeing now in the
            // current copy model)
            double probabilityOfCorrectPrediction;

            // Hits and misses info for the current copy model
            HitsMissesInfo hitsMissesInfo;

            // Expand window until we reach the end of the file of the probability reaches a certain threshold
            do {

                char predictedChar = fileReader.getCache()->operator[](pastSequencePosition);
                char nextCharacterInSequence = fileReader.getNextCharacterInSequence();

                // Calculate the probability P
                probabilityOfCorrectPrediction = calculateHitProbability(hitsMissesInfo.getHits(),
                                                                         hitsMissesInfo.getMisses(), alpha);

                // Stop the copy model if we have reach the threshold
                // if (probabilityOfCorrectPrediction < threshold) break;

                std::string lastWindowAsString = convertCharArrayToString(fileReader.getLastWindowContent(),
                                                                          fileReader.getWindowSize());

                // If we saw this sequence before, just add this new position
                pastSequencesPositions[lastWindowAsString].push_back(fileReader.getCurrentPosition());

                // Value to distribute for other characters
                double complementaryProbability = 1-probabilityOfCorrectPrediction;

                // Calculate the total amount of information
                // totalAmountOfInformation += -std::log2(probabilityOfCorrectPrediction);

                // If it turns out the next character in the copy model to be equal to the next character in the
                // sequence then we have a hit.
                if (predictedChar == nextCharacterInSequence) {

                    hitsMissesInfo.incrementHits();

                    // The total information is the sum of the information of each character at that point taking into
                    // account the probability of the character being correct
                    totalAmountOfInformation += -std::log2(probabilityOfCorrectPrediction);

                } else { // Otherwise, next character in sequence != of character in copy model, we have a miss

                    hitsMissesInfo.incrementMisses();

                    // The total information is the sum of the information of each character at that point taking into
                    // account the probability of the character being correct
                    totalAmountOfInformation += -std::log2(getProbabilityDistributionForCharacter(nextCharacterInSequence,
                                                                                                  complementaryProbability));

                }

            } while (fileReader.nextCharacter());

            // When we leave the loop we are stopping the copy model!
            std::string correspondingWindow = convertCharVectorToString(*fileReader.getCurrentSequence()).substr(0, fileReader.getWindowSize());
            std::vector<char> currentSequenceVector(fileReader.getCurrentSequence()->size());

            std::copy(fileReader.getCurrentSequence()->begin(), fileReader.getCurrentSequence()->end(), currentSequenceVector.begin());

            std::string currentSequence = convertCharVectorToString(currentSequenceVector);

            if (bestCopyForWindow.count(correspondingWindow) <= 0) {
                bestCopyForWindow.insert(std::make_pair(correspondingWindow, convertCharVectorToString(currentSequenceVector)));
            } else if (bestCopyForWindow[correspondingWindow].size() < currentSequenceVector.size()) {
                bestCopyForWindow[correspondingWindow] = convertCharVectorToString(currentSequenceVector);
            }

            // We are stopping this copy model because we reached the threshold
            if (probabilityOfCorrectPrediction < threshold && pastSequencesPositions[sequenceAsString].size() > 1) {

                std::random_device dev;
                std::mt19937 rng(dev());
                std::uniform_int_distribution<std::mt19937::result_type> dist(1,pastSequencesPositions[sequenceAsString].size()-1); // distribution in range [1, 6]

                // Change the pointer to the next one
                // currentPointerIndexForSequence[sequenceAsString] = (int) dist(rng);
                currentPointerIndexForSequence[sequenceAsString] += 1;

            }

        } else {

            // Once we can't initialize the copy model, we do a uniform distribution of probabilities for all characters
            // calculate the information and add it to the total amount of information
            totalAmountOfInformation += -std::log2(1.0/(int)fileInfo.getAlphabet().size());

            // If we have never seen this sequence, add it as key and a vector with the position
            pastSequencesPositions.insert(std::pair<std::string, std::vector<int>>(sequenceAsString, {fileReader.getCurrentPosition()}));

        }

        std::printf("Progress (Growing Window Model): %d%%\r", getProgress());
        std::fflush(stdout);

    }

    fileReader.closeFile();

}

double GrowingWindowModelBuilder::calculateInformationByCharacter() {
    return totalAmountOfInformation/fileInfo.getSize();
}

double GrowingWindowModelBuilder::calculateTotalInformation() {
    return totalAmountOfInformation;
}

std::map<std::string, std::string> GrowingWindowModelBuilder::getModel() {
    return bestCopyForWindow;
}
