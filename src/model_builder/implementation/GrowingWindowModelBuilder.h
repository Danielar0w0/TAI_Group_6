#ifndef SRC_GROWINGWINDOWMODELBUILDER_H
#define SRC_GROWINGWINDOWMODELBUILDER_H

#include <map>
#include <cmath>
#include "../abstract/AbstractModelBuilder.h"
#include "../../utils/generalUtils.h"
#include "../utils/HitsMissesInfo.h"

class GrowingWindowModelBuilder : public AbstractModelBuilder {

    std::map<std::string, std::vector<int>> pastSequencesPositions;
    std::map<std::string, int> currentPointerIndexForSequence;
    std::map<std::string, std::string> bestCopyForWindow;

    double totalAmountOfInformation = 0;

public:

    GrowingWindowModelBuilder(const FileReader &fileReader, const FileInfo &fileInfo, const Logger &logger);

    ~GrowingWindowModelBuilder() override;

    void buildModel(double alpha, double threshold) override;

    double calculateInformationByCharacter() override;

    double calculateTotalInformation() override;

    std::map<std::string, std::string> getModel();

};


#endif
