#ifndef SRC_ABSTRACTMODELBUILDER_H
#define SRC_ABSTRACTMODELBUILDER_H

#include <utility>
#include <cmath>

#include "../../reader/FileReader.h"
#include "../../debugging/Logger.h"

class AbstractModelBuilder {

protected:
    FileReader fileReader;
    FileInfo fileInfo;
    Logger logger;
    std::map<char, double> symbolsDistribution;

    int getProgress();

public:

    AbstractModelBuilder(FileReader fileReader, FileInfo fileInfo, Logger logger);
    virtual ~AbstractModelBuilder();

    virtual void buildModel(double alpha, double threshold) = 0;

    virtual double getProbabilityDistributionForCharacter(char character, double complementaryProbability);

};


#endif
