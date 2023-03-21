#include "AbstractModelBuilder.h"

AbstractModelBuilder::AbstractModelBuilder(FileReader fileReader, FileInfo fileInfo) : fileReader(std::move(
        fileReader)), fileInfo(std::move(fileInfo)) {}

int AbstractModelBuilder::getProgress() {
    return (int) std::round(fileReader.getCurrentPosition()*100/fileInfo.getSize());
}

AbstractModelBuilder::~AbstractModelBuilder() = default;
