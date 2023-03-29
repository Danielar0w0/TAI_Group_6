#include <random>
#include "CopyModelBuilder.h"

void CopyModelBuilder::buildModel(double alpha, double threshold) {

    FILE* exportInformation = fopen("information.txt", "w");

    fileReader.openFile();

    int counter = 0;

    // Read a window
    while (fileReader.next()) {

        // Get Window sequence as string
        std::string sequenceAsString = convertCharArrayToString(fileReader.getWindowContent(), fileReader.getWindowSize());

        logger.debug("Reading Window " + sequenceAsString + " on position " + std::to_string(fileReader.getCurrentPosition()));

        // We have seen this sequence in the past
        if (pastSequencesPositions.count(sequenceAsString) > 0 && pastSequencesPositions[sequenceAsString][0] < fileReader.getCurrentPosition()-1) {

            int currentPointerIndex;

            // If I haven't chosen a pointer for this sequence yet, chose the first pointer
            if (currentPointerIndexForSequence.count(sequenceAsString) <= 0) {
                currentPointerIndex = 0;
                currentPointerIndexForSequence.insert(std::make_pair(sequenceAsString, currentPointerIndex));
            } else {
                currentPointerIndex = currentPointerIndexForSequence[sequenceAsString];
            }

            // If we saw this sequence before, just add this new position if it doesn't exists already
            if (std::find(pastSequencesPositions[sequenceAsString].begin(),
                          pastSequencesPositions[sequenceAsString].end(),fileReader.getCurrentPosition()) == pastSequencesPositions[sequenceAsString].end())
                pastSequencesPositions[sequenceAsString].push_back(fileReader.getCurrentPosition());

            int pastSequencePosition = pastSequencesPositions[sequenceAsString][currentPointerIndex];

            // The probability P (probability of the next character being equal to the one I'm seeing now in the
            // current copy model)
            double probabilityOfCorrectPrediction;

            // Hits and misses info for the current copy model
            HitsMissesInfo hitsMissesInfo;

            int count = 0;

            // Expand window until we reach the end of the file of the probability reaches a certain threshold
            do {

                // int pastSequenceOffset = (int)fileReader.getCurrentSequence()->size() - (fileReader.getWindowSize()-1);
                char predictedChar = fileReader.getCache()->operator[](pastSequencePosition + count++);
                char nextCharacterInSequence = fileReader.getNextCharacterInSequence();

                // Calculate the probability P (a.k.a. probability of Hit)
                probabilityOfCorrectPrediction = calculateHitProbability(hitsMissesInfo.getHits(),
                                                                         hitsMissesInfo.getMisses(), alpha);

                std::string lastWindowAsString = convertCharArrayToString(fileReader.getLastWindowContent(),
                                                                          fileReader.getWindowSize());

                // If we saw this sequence before, just add this new position if it doesn't exists already
                if (std::find(pastSequencesPositions[lastWindowAsString].begin(),
                              pastSequencesPositions[lastWindowAsString].end(),fileReader.getCurrentPosition()+1) == pastSequencesPositions[lastWindowAsString].end()) {
                    pastSequencesPositions[lastWindowAsString].push_back(fileReader.getCurrentPosition()+1);
                }

                // Value to distribute for other characters
                double complementaryProbability = 1-probabilityOfCorrectPrediction;

                std::string currentSequence;

                if (logger.getLevel() >= LoggingLevel::DEBUG) {
                    currentSequence = convertCharArrayToString(fileReader.getCurrentSequence()->data(),
                                                               (int)fileReader.getCurrentSequence()->size());
                }

                // If it turns out the next character in the copy model to be equal to the next character in the
                // sequence then we have a hit.
                if (predictedChar == nextCharacterInSequence) {

                    hitsMissesInfo.incrementHits();

                    logger.debug("\n");
                    logger.debug("Hit on Character " + std::string(1, nextCharacterInSequence));
                    logger.debug("Current sequence is: " + currentSequence);
                    logger.debug("Past pointer at " + std::to_string(pastSequencePosition+count));
                    logger.debug("Probability: " + std::to_string(probabilityOfCorrectPrediction));

                    // The total information is the sum of the information of each character at that point taking into
                    // account the probability of the character being correct
                    totalAmountOfInformation += -std::log2(probabilityOfCorrectPrediction);

                    fprintf(exportInformation, "%f;%i\n", -std::log2(probabilityOfCorrectPrediction), counter++);

                } else { // Otherwise, next character in sequence != of character in copy model, we have a miss

                    hitsMissesInfo.incrementMisses();

                    double probabilityOfFail = complementaryProbability/(int)(fileInfo.getAlphabet().size()-1);

                    logger.debug("\n");
                    logger.debug("Miss on Character " + std::string(1, nextCharacterInSequence));
                    logger.debug("  Was expecting instead an " + std::string(1, predictedChar));
                    logger.debug("Current sequence is " + currentSequence);
                    logger.debug("Past pointer at " + std::to_string(pastSequencePosition+count));
                    logger.debug("Probability: " + std::to_string(probabilityOfFail));

                    // The total information is the sum of the information of each character at that point taking into
                    // account the probability of the character being correct
                    totalAmountOfInformation += -std::log2(probabilityOfFail);

                    fprintf(exportInformation, "%f;%i\n", -std::log2(probabilityOfFail), counter++);

                }

                std::printf("Building Copy Model... %d%%\r", getProgress());
                std::fflush(stdout);

            } while (probabilityOfCorrectPrediction >= threshold && fileReader.nextCharacter());

            // When we leave the loop we are stopping the copy model!

            // We are stopping this copy model because we reached the threshold
            if (probabilityOfCorrectPrediction < threshold && pastSequencesPositions[sequenceAsString].size() > 1) {

                // Change the pointer to the next one
                currentPointerIndexForSequence[sequenceAsString] += 1;

                // Read next character to shift next window
                fileReader.nextCharacter();

                logger.debug("Changed pointer!");

            }

        } else {

            // Once we can't initialize the copy model, we do a uniform distribution of probabilities for all characters
            // calculate the information and add it to the total amount of information
            totalAmountOfInformation += -std::log2(1.0/(int)fileInfo.getAlphabet().size());

            fprintf(exportInformation, "%f;%i\n", -std::log2(1.0/(int)fileInfo.getAlphabet().size()), counter++);

            // If we have never seen this sequence, add it as key and a vector with the position
            pastSequencesPositions.insert(std::pair<std::string, std::vector<int>>(sequenceAsString, {fileReader.getCurrentPosition()}));

        }

    }

    std::printf("\n\n");
    std::fflush(stdout);

    fclose(exportInformation);

    fileReader.closeFile();

}

CopyModelBuilder::~CopyModelBuilder() = default;

double CopyModelBuilder::calculateInformationByCharacter() {
    return totalAmountOfInformation/fileInfo.getSize();
}

double CopyModelBuilder::calculateTotalInformation() const {
    return totalAmountOfInformation;
}

CopyModelBuilder::CopyModelBuilder(const FileReader &fileReader, const FileInfo &fileInfo) : InputProcessorEntity(
        fileReader, fileInfo) {}

