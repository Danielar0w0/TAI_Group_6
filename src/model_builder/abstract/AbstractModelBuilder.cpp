#include "AbstractModelBuilder.h"

AbstractModelBuilder::AbstractModelBuilder(FileReader fileReader, FileInfo fileInfo) : fileReader(std::move(
        fileReader)), fileInfo(std::move(fileInfo)) {
    this->symbolsDistribution = this->fileInfo.getSymbolsDistribution();
}

int AbstractModelBuilder::getProgress() {
    return (int) std::round(fileReader.getCurrentPosition()*100/fileInfo.getSize());
}

double AbstractModelBuilder::getProbabilityDistributionForCharacter(char character, double complementaryProbability) {
    return complementaryProbability/(double) (fileInfo.getAlphabet().size()-1);
}


AbstractModelBuilder::~AbstractModelBuilder() = default;
