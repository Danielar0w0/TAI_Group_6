#include "FileReader.h"

#include <utility>
#include <iostream>

FileReader::FileReader(std::string filePath, int windowSize) {

    this->filePath = std::move(filePath);
    this->windowSize = windowSize;

    this->currentPosition = 0;
    this->currentWindow = new char[windowSize];

}

FILE* FileReader::openFile() {

    // Open the target sequence file
    FILE* target = fopen(filePath.c_str(), "r");

    if (target == nullptr) {
        std::cerr << "Error reading text file " << filePath.c_str() << ": " << stderr << std::endl;
        return nullptr;
    }

    this->targetFile = target;
    return this->targetFile;

}

char* FileReader::getWindowContent() {
    return this->currentWindow;
}

bool FileReader::next() {

    if (this->currentPosition == 0) {
        FileReader::readInitialWindow();
    } else {
        return FileReader::shiftWindow();
    }

    return true;

}

void FileReader::closeFile() {
    if (this->targetFile != nullptr)
        fclose(this->targetFile);
}


void FileReader::readInitialWindow() {

    if (this->targetFile == nullptr)
        throw std::runtime_error("Target file is a null pointer!");

    char character;

    for (int i = 0; i < this->windowSize; ++i) {

        if ((character = (char) fgetc(this->targetFile)) == EOF)
            throw std::runtime_error("The file is too short for the defined window size!");
        this->currentWindow[i] = character;
        this->cache.push_back(character);

    }

    this->futureCharacter = (char) fgetc(this->targetFile);
    this->cache.push_back(character);

    this->currentPosition = this->windowSize;

}

bool FileReader::shiftWindow() {

    if (this->targetFile == nullptr)
        throw std::runtime_error("Target file is a null pointer!");

    char character = (char) fgetc(this->targetFile);

    if (character == EOF)
        return false;

    // Shift the buffer
    for (int i = 0; i < this->windowSize-1; ++i)
        this->currentWindow[i] = this->currentWindow[i+1];

    this->currentWindow[this->windowSize-1] = this->futureCharacter;

    this->futureCharacter = character;
    this->cache.push_back(character);

    ++this->currentPosition;

    return true;

}

FileInfo FileReader::getFileInfo() {

    char currentCharacter;

    std::set<char> alphabet;
    int fileSize = 0;

    if (this->targetFile == nullptr)
        throw std::runtime_error("Target file is a null pointer!");

    while ((currentCharacter = (char) fgetc(this->targetFile)) != EOF) {
        alphabet.insert(currentCharacter);
        ++fileSize;
    }

    return FileInfo(alphabet, fileSize);

}

int FileReader::getWindowSize() const {
    return this->windowSize;
}

int FileReader::getCurrentPosition() const {
    return this->currentPosition;
}

std::vector<char> FileReader::getCache() {
    return this->cache;
}

char FileReader::getFutureCharacter() const {
    return this->futureCharacter;
}



