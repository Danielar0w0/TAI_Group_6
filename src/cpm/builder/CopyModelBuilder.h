#ifndef SRC_COPYMODELBUILDER_H
#define SRC_COPYMODELBUILDER_H

#include <map>
#include <cmath>
#include "../../utils/generalUtils.h"
#include "../support/HitsMissesInfo.h"
#include "../../reader/FileReader.h"
#include "../../utils/LoggableEntity.h"
#include "../../utils/InputProcessorEntity.h"

class CopyModelBuilder : public LoggableEntity, public InputProcessorEntity {

    std::map<std::string, std::vector<int>> pastSequencesPositions;
    std::map<std::string, int> currentPointerIndexForSequence;

    double totalAmountOfInformation = 0;

public:

    explicit CopyModelBuilder(const FileReader &fileReader, const FileInfo &fileInfo);

    ~CopyModelBuilder() override;

    void buildModel(double alpha, double threshold);
    double calculateInformationByCharacter();
    [[nodiscard]] double calculateTotalInformation() const;

};


#endif
