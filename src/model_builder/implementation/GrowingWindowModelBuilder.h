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
    std::map<char, double> probabilitiesForCharacter;
    std::map<char, int> characterProbabilitiesCount;
    std::map<std::string, std::string> bestCopyForWindow;

public:

    GrowingWindowModelBuilder(const FileReader &fileReader, const FileInfo &fileInfo);

    ~GrowingWindowModelBuilder() override;

    void buildModel(double alpha, double threshold) override;

    std::map<char, double> calculateInformationByCharacter() override;

    double calculateTotalInformation() override;

    std::map<std::string, std::string> getModel();

};


#endif
