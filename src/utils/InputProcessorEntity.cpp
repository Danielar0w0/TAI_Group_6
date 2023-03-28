#include <cmath>
#include "InputProcessorEntity.h"

int InputProcessorEntity::getProgress() {
    return (int) std::round(fileReader.getCurrentPosition()*100/fileInfo.getSize());
}

InputProcessorEntity::InputProcessorEntity(FileReader fileReader, FileInfo fileInfo)
    : fileInfo(std::move(fileInfo)), fileReader(std::move(fileReader)) {}

FileReader InputProcessorEntity::getFileReader() const {
    return this->fileReader;
}

FileInfo InputProcessorEntity::getFileInfo() const {
    return this->fileInfo;
}
