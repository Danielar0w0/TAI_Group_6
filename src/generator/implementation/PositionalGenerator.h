#ifndef SRC_POSITIONALGENERATOR_H
#define SRC_POSITIONALGENERATOR_H


#include "../abstract/AbstractGenerator.h"
#include "../../serialization/implementation/PositionalModelSerializer.h"
#include "../../reader/RandomAccessReader.h"

class PositionalGenerator : public AbstractGenerator {

    PositionalModelSerializer positionalModelSerializer;

public:

    explicit PositionalGenerator(PositionalModelSerializer positionalModelSerializer);

    void generateTextInteractively(int generationSize) override;
    void generateTextOnce(int generationSize) override;
    int getModelWindowSize() override;

    char generateNextCharacter(const std::string &currentWindow, RandomAccessReader randomAccessReader);

private:

    std::string generateText(int generationSize, const std::string& initialText, const std::map<std::string, std::vector<int>>& model, bool realTime);

};


#endif
