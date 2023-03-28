#ifndef SRC_FILEINFO_H
#define SRC_FILEINFO_H

#include <set>
#include <map>
#include <string>

class FileInfo {

    std::string filePath;
    std::set<char> alphabet;
    std::map<char, int> symbolsCount;
    int size;

    public:

        explicit FileInfo(const std::string& filePath, std::set<char> alphabet, std::map<char, int> symbolsCount, int size);

        std::set<char> getAlphabet();
        std::map<char, int> getSymbolsCount();
        std::map<char, double> getSymbolsDistribution();
        [[nodiscard]] int getSize() const;
        [[nodiscard]] std::string getFilePath() const;

};


#endif //SRC_FILEINFO_H
