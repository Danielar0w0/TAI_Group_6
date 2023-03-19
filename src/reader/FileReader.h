//
// Created by hugo on 05-03-2023.
//

#ifndef SRC_FILEREADER_H
#define SRC_FILEREADER_H

#include <string>
#include <vector>
#include "FileInfo.h"

class FileReader {

    std::string filePath;
    int windowSize;

    FILE* targetFile{};
    int currentPosition;
    char* currentWindow;
    char futureCharacter{};

    std::vector<char> cache;

    public:

        explicit FileReader(std::string filePath, int windowSize);

        FILE* openFile();
        void closeFile();

        bool next();

        FileInfo getFileInfo();

        char* getWindowContent();
        std::vector<char>* getCache();

        [[nodiscard]] int getWindowSize() const;
        [[nodiscard]] int getCurrentPosition() const;

        char getFutureCharacter() const;

    private:

        void readInitialWindow();
        bool shiftWindow();

};


#endif //SRC_FILEREADER_H
