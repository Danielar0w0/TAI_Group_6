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
    char* lastWindow;
    std::vector<char> currentSequence;
    char nextCharacterInSequence{};

    std::vector<char> cache;

    public:

        explicit FileReader(std::string filePath, int windowSize);

        FILE* openFile();
        void closeFile();

        bool next();
        bool nextCharacter();

        FileInfo getFileInfo();

        char* getWindowContent();
        char* getLastWindowContent();

        std::vector<char>* getCurrentSequence();
        std::vector<char>* getCache();

        [[nodiscard]] int getWindowSize() const;
        [[nodiscard]] int getCurrentPosition() const;

        [[nodiscard]] char getNextCharacterInSequence() const;

    private:

        void readInitialWindow();
        bool shiftWindow();
        void resetCurrentSequence();

};


#endif //SRC_FILEREADER_H
