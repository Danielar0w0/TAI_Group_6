#include <iostream>
#include <string>
#include <cmath>

#include "utils/printUtils.h"
#include "input/classes/InputArguments.h"
#include "input/InputUtils.h"
#include "reader/FileReader.h"

using namespace std;

// Develop a program, named cpm, that implements a copy model.
// All required parameters to control the model must be passed as command line arguments.

// One major advantage here is the ability to adjust the probabilistic model as the encoding proceeds,
// in response to the changing probability distribution from one encoded symbol to the next.

// Threshold for the copy model
double THRESHOLD = 0.5;

void analyseFileSequence(FileReader fileReader, FileInfo fileInfo, double alpha);
FileInfo getFileInfo(const InputArguments& inputArguments);
FileReader getFileReaderInstance(const InputArguments& inputArguments);


int main(int argc, char *argv[]) {

    // Read command line arguments
    InputArguments inputArguments = getInputArguments(argc, argv);

    bool areArgumentsValid = inputArguments.checkArguments();

    if (!areArgumentsValid)
        std::exit(EXIT_FAILURE);


    // First Pass: Get File Info. (Alphabet and size)
    FileInfo fileInfo = getFileInfo(inputArguments);

    // TODO: Remove this print
    printAlphabet(fileInfo.getAlphabet());

    cout << endl;

    // Get File Reader instance for second pass
    FileReader fileReader = getFileReaderInstance(inputArguments);

    analyseFileSequence(fileReader, fileInfo, inputArguments.getAlpha());


}

void analyseFileSequence(FileReader fileReader, FileInfo fileInfo, double alpha) {

    std::map<string, vector<int>> pastSequencesPositions;
    std::map<string, int> sequenceCurrentPointer;
    std::map<string, int> sequenceCurrentPointerIndex;

    // Todo: We may only need to store the hits and misses for the character we are currently predicting
    std::map<string, std::map<char, int*>> hitsAndMissesForEachSequence;

    std::map<char, double> probabilitiesByCharacter;

    fileReader.openFile();

    int predictionsCount = 0;

    while (fileReader.next()) {

        // printSequence(fileReader.getWindowContent(), fileReader.getWindowSize());

        string sequenceAsString(fileReader.getWindowContent());

        // Let's try to predict

        if (pastSequencesPositions.count(sequenceAsString) > 0) { // We can predict because we have already seen it

            // Have we already selected a pointer for this sequence??

            if (sequenceCurrentPointer.count(sequenceAsString) <= 0) { // We have not selected a pointer for the current sequence. Select one (the first one)!!
                sequenceCurrentPointer[sequenceAsString] = pastSequencesPositions[sequenceAsString].rbegin()[0];
                sequenceCurrentPointerIndex[sequenceAsString] = 0;
            }

            // Now we have selected a pointer for this sequence for sure.
            int currentPointerForCurrentSequence = sequenceCurrentPointer[sequenceAsString];

            // Which caracter are we predicting that will come next?
            char predictedCharacter = fileReader.getCache()[currentPointerForCurrentSequence+1];

            // Which caracter was indeed next?
            char futureCharacter = fileReader.getFutureCharacter();

            // Have we already calculated any probability for this sequence?
            // For this sequence, have we calculated any probability for a certain character?
            if (hitsAndMissesForEachSequence.count(sequenceAsString) <= 0
                || hitsAndMissesForEachSequence[sequenceAsString].count(predictedCharacter) <= 0) { // No. We need to initialize it

                // The first element is a Hit, second a Miss
                // Todo: Change this to a struct or class
                int hitsAndMissesArray[2];

                if (predictedCharacter == futureCharacter) { // Then we have a hit! :)
                    // Set 1 Hit and 0 Misses
                    hitsAndMissesArray[0] = 1;
                    hitsAndMissesArray[1] = 0;
                } else {
                    // Set 0 Hits and 1 Miss
                    hitsAndMissesArray[0] = 0;
                    hitsAndMissesArray[1] = 1;
                }

                // We initialized the hits and misses
                // hitsAndMissesForEachSequence[sequenceAsString][predictedCharacter] = hitsAndMissesArray;
                hitsAndMissesForEachSequence[sequenceAsString].insert(std::make_pair(predictedCharacter, hitsAndMissesArray));

            } else { // Yes, the hits and misses are already initialized. Just change it

                // Get the hits and misses, add one hit, and re-assign it
                int* hitsAndMissesArray = hitsAndMissesForEachSequence[sequenceAsString][predictedCharacter];

                if (predictedCharacter == futureCharacter) { // Then we have a hit! :)
                    ++hitsAndMissesArray[0]; // Add one Hit
                } else {
                    ++hitsAndMissesArray[1]; // Add one miss
                }

                hitsAndMissesForEachSequence[sequenceAsString][predictedCharacter] = hitsAndMissesArray; // Re-assign it

            }

            // Now, we will calculate the probability for our predicted character

            int hitsForPredictedCharacter = hitsAndMissesForEachSequence[sequenceAsString][predictedCharacter][0];
            int missesForPredictedCharacter = hitsAndMissesForEachSequence[sequenceAsString][predictedCharacter][1];
            double probabilityForPredictedCharacter = calculateHitProbability(hitsForPredictedCharacter, missesForPredictedCharacter, alpha);

            // If our probability is below our threshold we need to change the pointer for our sequence because the current pointer is bad!
            if (probabilityForPredictedCharacter < THRESHOLD) {

                // Change the pointer to another one
                int nextPointerIndex = ++sequenceCurrentPointerIndex[sequenceAsString];
                sequenceCurrentPointer[sequenceAsString] = pastSequencesPositions[sequenceAsString].rbegin()[nextPointerIndex];

                // Clean hits and misses for this sequence and this prediction
                int hitsAndMissesArray[2] = {0,0};
                hitsAndMissesForEachSequence[sequenceAsString][predictedCharacter] = hitsAndMissesArray;

            }

            // Regardless, we need to update the amount of information for each character in our alphabet.
            probabilitiesByCharacter[predictedCharacter] += probabilityForPredictedCharacter;

            ++predictionsCount;

            // Here we do a uniform distribution of probabilities across the different characters of the alphabet
            double probabilityDistribution = (1-probabilityForPredictedCharacter)/(int)(fileInfo.getAlphabet().size()-1);

            // And assign the calculated probability to each one of the characters
            for (char characterInAlphabet : fileInfo.getAlphabet()) {
                if (characterInAlphabet != predictedCharacter)
                    probabilitiesByCharacter[characterInAlphabet] += probabilityDistribution;
            }

            // Todo: For a given sequence, store the probabilities for all the characters and in the end maybe do the average of the probabilities?

        }

        // Independently of predicting or not something, we will want to store this sequence and the position after the sequence

        if (pastSequencesPositions.count(sequenceAsString) <= 0) { // If we have never seen this sequence, add it as key and a vector with the position
            pastSequencesPositions.insert(pair<string,vector<int>>(sequenceAsString, {fileReader.getCurrentPosition()}));
        } else { // If we saw it, just add this new position
            pastSequencesPositions[sequenceAsString].push_back(fileReader.getCurrentPosition());
        }

        printf("Progress: %f\r", std::round(fileReader.getCurrentPosition()*100/fileInfo.getSize()));
        fflush(stdout);

    }

    for (auto i : probabilitiesByCharacter) {
        // cout << i.first << " : " << i.second/predictionsCount << endl;
        cout << i.first << " : " << -log2(i.second / predictionsCount) << endl;
    }

    fileReader.closeFile();

}

FileInfo getFileInfo(const InputArguments& inputArguments) {

    FileReader fileReader = FileReader(inputArguments.getFilePath(), inputArguments.getK());

    fileReader.openFile();
    FileInfo fileInfo = fileReader.getFileInfo();
    fileReader.closeFile();

    return fileInfo;

}

FileReader getFileReaderInstance(const InputArguments& inputArguments) {
    return FileReader(inputArguments.getFilePath(), inputArguments.getK());
}




