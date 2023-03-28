#ifndef SRC_INPUTPROCESSORENTITY_H
#define SRC_INPUTPROCESSORENTITY_H


#include "../reader/FileReader.h"

class InputProcessorEntity {

protected:
    FileReader fileReader;
    FileInfo fileInfo;

public:

    explicit InputProcessorEntity(FileReader fileReader, FileInfo fileInfo);

    int getProgress();

    FileReader getFileReader() const;
    FileInfo getFileInfo() const;

};


#endif
