#ifndef SRC_RANDOMACCESSREADER_H
#define SRC_RANDOMACCESSREADER_H


#include <string>

class RandomAccessReader {

    std::string filePath;
    FILE* targetFile{};

public:

    explicit RandomAccessReader(std::string filePath);

    FILE* openFile();
    void closeFile();

    char getCharAt(int position);


};


#endif //SRC_RANDOMACCESSREADER_H
