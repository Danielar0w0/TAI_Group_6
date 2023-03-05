#include "FileInfo.h"

#include <utility>

FileInfo::FileInfo(std::set<int> alphabet, int size) {
    this->alphabet = std::move(alphabet);
    this->size = size;
}

std::set<int> FileInfo::getAlphabet() {
    return this->alphabet;
}

int FileInfo::getSize() const {
    return this->size;
}

