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

            // The probability P (probability of the character I'm seeing now being the correct one accordingly to
            // the current copy model)
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

                // Save the probability for the next character in the sequence
                //
                // Remainder: the probability P is the probability of the character I'm seeing now being the correct one
                // accordingly to the current copy model
                probabilitiesForCharacter[nextCharacterInSequence] += probabilityOfCorrectPrediction;
                characterProbabilitiesCount[nextCharacterInSequence]++;

                // Value to distribute for other characters
                double probDistributionForCharacter = (1 - probabilityOfCorrectPrediction) / (double) (fileInfo.getAlphabet().size() - 1);

                // And assign the calculated probability to each one of the characters
                for (char characterInAlphabet : fileInfo.getAlphabet()) {
                    if (characterInAlphabet != nextCharacterInSequence) {
                        probabilitiesForCharacter[characterInAlphabet] += probDistributionForCharacter;
                        characterProbabilitiesCount[characterInAlphabet]++;
                    }
                }

                // Calculate the total amount of information
                // totalAmountOfInformation += -std::log2(probabilityOfCorrectPrediction);

                // If it turns out the next character in the copy model to be equal to the next character in the
                // sequence then we have a hit.
                if (predictedChar == nextCharacterInSequence) {
                    hitsMissesInfo.incrementHits();
                } else { // Otherwise, next character in sequence != of character in copy model, we have a miss
                    hitsMissesInfo.incrementMisses();
                }

            } while (probabilityOfCorrectPrediction > threshold && fileReader.nextCharacter());

            // When we leave the loop we are stopping the copy model!
            std::string correspondingWindow = convertCharVectorToString(*fileReader.getCurrentSequence()).substr(0, fileReader.getWindowSize());
            std::vector<char> currentSequenceVector(fileReader.getCurrentSequence()->size());

            std::copy(fileReader.getCurrentSequence()->begin(), fileReader.getCurrentSequence()->end(), currentSequenceVector.begin());

            if (bestCopyForWindow.count(correspondingWindow) <= 0) {
                bestCopyForWindow.insert(std::make_pair(correspondingWindow, convertCharVectorToString(currentSequenceVector)));
            } else if (bestCopyForWindow[correspondingWindow].size() < currentSequenceVector.size()) {
                bestCopyForWindow[correspondingWindow] = convertCharVectorToString(currentSequenceVector);
            }

            // We are stopping this copy model because we reached the threshold
            if (probabilityOfCorrectPrediction <= threshold && pastSequencesPositions[sequenceAsString].size() > 1) {
                // Change the pointer to the next one
                ++currentPointerIndexForSequence[sequenceAsString];
            }


        } else {
            // If we have never seen this sequence, add it as key and a vector with the position
            pastSequencesPositions.insert(std::pair<std::string, std::vector<int>>(sequenceAsString, {fileReader.getCurrentPosition()}));
        }

        printf("Progress: %f\r", std::round(fileReader.getCurrentPosition()*100/fileInfo.getSize()));
        fflush(stdout);

    }

    fileReader.closeFile();

}

std::map<char, double> GrowingWindowModelBuilder::calculateInformationByCharacter() {

    std::map<char, double> informationByCharacter;

    for (const auto& i : probabilitiesForCharacter) {

        char character = i.first;
        double probabilitiesSum = i.second;
        double probabilitiesCount = characterProbabilitiesCount[character];

        double average = probabilitiesSum/probabilitiesCount;

        informationByCharacter[character] = average;

    }

    return informationByCharacter;

}

double GrowingWindowModelBuilder::calculateTotalInformation() {

    double totalAmountOfInformation = 0;

    for (const auto& i : probabilitiesForCharacter) {

        char character = i.first;
        double probabilitiesSum = i.second;
        double probabilitiesCount = characterProbabilitiesCount[character];

        double average = probabilitiesSum/probabilitiesCount;

        totalAmountOfInformation += fileInfo.getSymbolsCount()[character] * -std::log2(average);

    }

    return totalAmountOfInformation;

}

std::map<std::string, std::string> GrowingWindowModelBuilder::getModel() {
    return bestCopyForWindow;
}
