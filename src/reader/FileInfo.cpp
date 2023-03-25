#include "FileInfo.h"

#include <utility>

FileInfo::FileInfo(std::set<char> alphabet, std::map<char, int> symbolsCount, int size) {
    this->alphabet = std::move(alphabet);
    this->symbolsCount = std::move(symbolsCount);
    this->size = size;
}

std::set<char> FileInfo::getAlphabet() {
    return this->alphabet;
}

std::map<char, int> FileInfo::getSymbolsCount() {
    return this->symbolsCount;
}

std::map<char, double> FileInfo::getSymbolsDistribution() {

    std::map<char, double> symbolsDistribution;

    for (auto &symbol : this->symbolsCount)
        symbolsDistribution[symbol.first] = (double) symbol.second / this->size;

    return symbolsDistribution;

}

int FileInfo::getSize() const {
    return this->size;
}

