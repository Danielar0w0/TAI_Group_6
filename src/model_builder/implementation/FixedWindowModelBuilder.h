#ifndef SRC_FIXEDWINDOWMODELBUILDER_H
#define SRC_FIXEDWINDOWMODELBUILDER_H

#include <map>
#include <utility>
#include <cmath>
#include <iostream>

#include "../../reader/FileReader.h"
#include "../utils/HitsMissesInfo.h"
#include "../../utils/generalUtils.h"
#include "../abstract/AbstractModelBuilder.h"

class FixedWindowModelBuilder : public AbstractModelBuilder {

    std::map<std::string, std::vector<int>> pastSequencesPositions;
    std::map<std::string, int> sequenceCurrentPointer;
    std::map<std::string, int> sequenceCurrentPointerIndex;

    std::map<std::string, std::pair<char, HitsMissesInfo>> hitsAndMissesForEachSequence;

    // Temporary variables to calculate the average of probabilities for each sequence + symbol
    std::map<std::string, std::map<char, double>> sequenceSymbolProbabilitiesSum;
    std::map<std::string, int> sequenceSymbolProbabilitiesCount;

    std::map<char, double> probabilitiesByCharacter;

    int totalAmountOfPredictions = 0;

public:

    FixedWindowModelBuilder(const FileReader &fileReader, const FileInfo &fileInfo, const Logger &logger);

    ~FixedWindowModelBuilder() override;


    void buildModel(double alpha, double threshold) override;
    double calculateInformationByCharacter() override;
    double calculateTotalInformation() override;

    std::map<std::string, std::map<char, double>> getModel();

};


#endif
