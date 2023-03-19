#ifndef SRC_SEQUENTIALFILEREADER_H
#define SRC_SEQUENTIALFILEREADER_H


#include <string>
#include <vector>

class SequentialFileReader {

    std::string filePath;
    int windowSize;

    FILE* targetFile{};
    int currentPosition;

    std::vector<char> cache;

    public:

        explicit SequentialFileReader(std::string filePath, int windowSize);


};


#endif
