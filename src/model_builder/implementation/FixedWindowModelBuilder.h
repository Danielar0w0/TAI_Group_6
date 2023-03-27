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
    std::map<std::string, std::map<char, double>> probabilitiesForSequence;



public:

    FixedWindowModelBuilder(const FileReader &fileReader, const FileInfo &fileInfo, const Logger &logger);

    ~FixedWindowModelBuilder() override;

    void buildModel(double alpha, double threshold) override;

    std::map<std::string, std::map<char, double>> getProbabilisticModel();
    std::map<std::string, std::vector<int>> getPositionModel();

};


#endif
