#ifndef SRC_FILEINFO_H
#define SRC_FILEINFO_H

#include <set>

class FileInfo {

    std::set<char> alphabet;
    int size;

    public:

        explicit FileInfo(std::set<char> alphabet, int size);

        std::set<char> getAlphabet();
        [[nodiscard]] int getSize() const;

};


#endif //SRC_FILEINFO_H
