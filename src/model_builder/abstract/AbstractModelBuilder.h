#ifndef SRC_ABSTRACTMODELBUILDER_H
#define SRC_ABSTRACTMODELBUILDER_H

#include <utility>
#include <cmath>

#include "../../reader/FileReader.h"

class AbstractModelBuilder {

protected:
    FileReader fileReader;
    FileInfo fileInfo;
    std::map<char, double> symbolsDistribution;

    int getProgress();

public:

    AbstractModelBuilder(FileReader fileReader, FileInfo fileInfo);
    virtual ~AbstractModelBuilder();

    virtual void buildModel(double alpha, double threshold) = 0;
    virtual double calculateInformationByCharacter() = 0;
    virtual double calculateTotalInformation() = 0;

    virtual double getProbabilityDistributionForCharacter(char character, double complementaryProbability);

};


#endif
