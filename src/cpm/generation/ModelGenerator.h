#ifndef SRC_MODELGENERATOR_H
#define SRC_MODELGENERATOR_H

#include <map>
#include <utility>
#include <cmath>
#include <iostream>

#include "../../reader/FileReader.h"
#include "../support/HitsMissesInfo.h"
#include "../../utils/generalUtils.h"
#include "../../utils/LoggableEntity.h"
#include "../../utils/InputProcessorEntity.h"

class ModelGenerator : public LoggableEntity, public InputProcessorEntity {

    std::map<std::string, std::vector<int>> pastSequencesPositions;
    std::map<std::string, std::map<char, double>> probabilitiesForSequence;

public:

    ModelGenerator(const FileReader &fileReader, const FileInfo &fileInfo);
    ~ModelGenerator() override;

    void generateModel();

    std::map<std::string, std::map<char, double>> getProbabilisticModel();
    std::map<std::string, std::vector<int>> getPositionModel();

};


#endif
