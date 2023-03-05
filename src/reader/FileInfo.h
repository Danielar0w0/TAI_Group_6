#ifndef SRC_FILEINFO_H
#define SRC_FILEINFO_H

#include <set>

class FileInfo {

    std::set<int> alphabet;
    int size;

    public:

        explicit FileInfo(std::set<int> alphabet, int size);

        std::set<int> getAlphabet();
        [[nodiscard]] int getSize() const;

};


#endif //SRC_FILEINFO_H
