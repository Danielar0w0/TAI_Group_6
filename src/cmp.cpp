#include <iostream>
#include <string>
#include <cmath>

#include "utils/printUtils.h"
#include "input/classes/InputArguments.h"
#include "input/InputUtils.h"
#include "reader/FileReader.h"
#include "hitsMisses/HitsMissesInfo.h"
#include "modelSerializer/ModelSerializer.h"

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

string convertToString(char* a, int size)
{
    int i;
    string s = "";
    for (i = 0; i < size; i++) {
        s = s + a[i];
    }
    return s;
}

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
    std::map<string, std::pair<char, HitsMissesInfo>> hitsAndMissesForEachSequence;

    std::map<string, std::map<char, double>> sequenceSymbolProbabilities;

    // Temporary variables to calculate the average of probabilities for each sequence + symbol
    std::map<string, std::map<char, double>> sequenceSymbolProbabilitiesSum;
    std::map<string, int> sequenceSymbolProbabilitiesCount;

    std::map<char, double> probabilitiesByCharacter;

    fileReader.openFile();

    int predictionsCount = 0;

    while (fileReader.next()) {

        // printSequence(fileReader.getWindowContent(), fileReader.getWindowSize());

        string sequenceAsString = convertToString(fileReader.getWindowContent(), fileReader.getWindowSize());

        // Let's try to predict

        if (pastSequencesPositions.count(sequenceAsString) > 0) { // We can predict because we have already seen it

            // Have we already selected a pointer for this sequence??

            if (sequenceCurrentPointer.count(sequenceAsString) <= 0) { // We have not selected a pointer for the current sequence. Select one (the first one)!!
                sequenceCurrentPointer[sequenceAsString] = pastSequencesPositions[sequenceAsString].rbegin()[0];
                sequenceCurrentPointerIndex[sequenceAsString] = 0;
            }

            // Now we have selected a pointer for this sequence for sure.
            int currentPointerForCurrentSequence = sequenceCurrentPointer[sequenceAsString];

            // Which character are we predicting that will come next?
            char predictedCharacter = fileReader.getCache()[currentPointerForCurrentSequence+1];

            // Which character was indeed next?
            char futureCharacter = fileReader.getFutureCharacter();

            HitsMissesInfo hitsMissesInfo = HitsMissesInfo();

            if (hitsAndMissesForEachSequence.count(sequenceAsString) > 0 &&
                hitsAndMissesForEachSequence[sequenceAsString].first == predictedCharacter) {

                hitsMissesInfo = hitsAndMissesForEachSequence[sequenceAsString].second;

            }

            if (predictedCharacter == futureCharacter) { // Then we have a hit! :)
                hitsMissesInfo.incrementHits(); // Add one Hit
            } else {
                hitsMissesInfo.incrementMisses(); // Add one miss
            }

            // hitsAndMissesForEachSequence[sequenceAsString].second = hitsMissesInfo; // Re-assign it
            hitsAndMissesForEachSequence[sequenceAsString] = std::make_pair(predictedCharacter, hitsMissesInfo);

            // Now, we will calculate the probability for our predicted character

            unsigned int hitsForPredictedCharacter = hitsAndMissesForEachSequence[sequenceAsString].second.getHits();
            unsigned int missesForPredictedCharacter = hitsAndMissesForEachSequence[sequenceAsString].second.getMisses();
            double probabilityForPredictedCharacter = calculateHitProbability(hitsForPredictedCharacter, missesForPredictedCharacter, alpha);

            // If our probability is below our threshold we need to change the pointer for our sequence because the current pointer is bad!
            if (probabilityForPredictedCharacter < THRESHOLD) {

                // Change the pointer to another one
                int nextPointerIndex = ++sequenceCurrentPointerIndex[sequenceAsString];
                sequenceCurrentPointer[sequenceAsString] = pastSequencesPositions[sequenceAsString].rbegin()[nextPointerIndex];

                // Clean hits and misses for this sequence and this prediction
                hitsAndMissesForEachSequence[sequenceAsString] = std::make_pair(predictedCharacter, HitsMissesInfo());

            }

            // Regardless, we need to update the amount of information for each character in our alphabet.
            probabilitiesByCharacter[predictedCharacter] += probabilityForPredictedCharacter;

            // Keep track of symbol probabilities for current sequence
            sequenceSymbolProbabilitiesSum[sequenceAsString][predictedCharacter] += probabilityForPredictedCharacter;

            ++predictionsCount;

            // Here we do a uniform distribution of probabilities across the different characters of the alphabet
            double probabilityDistribution = (1-probabilityForPredictedCharacter)/(int)(fileInfo.getAlphabet().size()-1);

            // And assign the calculated probability to each one of the characters
            for (char characterInAlphabet : fileInfo.getAlphabet()) {

                if (characterInAlphabet != predictedCharacter) {
                    probabilitiesByCharacter[characterInAlphabet] += probabilityDistribution;

                    // Keep track of symbol probabilities for current sequence
                    sequenceSymbolProbabilitiesSum[sequenceAsString][characterInAlphabet] += probabilityDistribution;
                }
            }

            // Update count to later calculate average of probabilities
            sequenceSymbolProbabilitiesCount[sequenceAsString]++;

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


    //  Calculate probabilities for the model to be used in the generator
    for (auto i: sequenceSymbolProbabilitiesSum) {
        string sequence = i.first;
        for (auto j: i.second) {
            char c = j.first; double prob = j.second;

            // Update final average of probabilities for sequence + symbol
            int count = sequenceSymbolProbabilitiesCount[sequence];
            sequenceSymbolProbabilities[sequence][c] = prob/count;
        }
    }

    ModelSerializer model = ModelSerializer(sequenceSymbolProbabilities);
    // model.printModel(3);
    model.outputModel("./model.txt"); // TODO: adjust file output, maybe command line arg?
    // cout << endl;

    map<char, int> symbolsCount = fileInfo.getSymbolsCount();
    for (auto i: symbolsCount) {
        cout << i.first << " : " << i.second << endl;
    }
    cout << endl;

    for (auto i : probabilitiesByCharacter) {
        // cout << i.first << " : " << i.second/predictionsCount << endl;
        cout << i.first << " : " << -log2(i.second / predictionsCount) << endl;
    }

    double totalInformation = 0; char c; int count;
    for (auto i: symbolsCount) {
        c = i.first; count = i.second;
        totalInformation = count * -log2(probabilitiesByCharacter[c] / predictionsCount);
    }
    cout << endl;
    cout << "Total Information: " << totalInformation << endl;

    fileReader.closeFile();

    // TODO: testing loading model, delete this later
    ModelSerializer model2 = ModelSerializer();
    model2.loadModel("./model.txt");
    cout << endl;
    model2.printModel(3);



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




