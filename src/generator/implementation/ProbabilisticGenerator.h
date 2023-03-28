#ifndef SRC_PROBABILISTICGENERATOR_H
#define SRC_PROBABILISTICGENERATOR_H

#include "../abstract/AbstractGenerator.h"
#include "../../serialization/implementation/ProbabilisticModelSerializer.h"

class ProbabilisticGenerator : public AbstractGenerator {

    ProbabilisticModelSerializer probabilisticModelSerializer;

public:

    explicit ProbabilisticGenerator(ProbabilisticModelSerializer probabilisticModelSerializer);

    void generateTextInteractively(int generationSize) override;
    void generateTextOnce(int generationSize) override;
    int getModelWindowSize() override;

private:

    std::string generateText(int generationSize, const std::string& initialText, const std::map<std::string, std::map<char, double>>& model, bool realTime);
    char generateNextCharacter(const std::string& currentWindow);
    static char getCharacterUsingUniformDist(const std::map<char, double>& charactersProbabilityDistribution);

};


#endif
