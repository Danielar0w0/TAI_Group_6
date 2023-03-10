#ifndef SRC_FILEINFO_H
#define SRC_FILEINFO_H

#include <set>
#include <map>

class FileInfo {

    std::set<char> alphabet;
    std::map<char, int> symbolsCount;
    int size;

    public:

        explicit FileInfo(std::set<char> alphabet, std::map<char, int> symbolsCount, int size);

        std::set<char> getAlphabet();
        std::map<char, int> getSymbolsCount();
        [[nodiscard]] int getSize() const;

};


#endif //SRC_FILEINFO_H
