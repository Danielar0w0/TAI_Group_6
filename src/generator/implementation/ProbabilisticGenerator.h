#ifndef SRC_PROBABILISTICGENERATOR_H
#define SRC_PROBABILISTICGENERATOR_H

#include "../abstract/AbstractGenerator.h"
#include "../../serialization/implementation/ProbabilisticModelSerializer.h"

class ProbabilisticGenerator : public AbstractGenerator {

    ProbabilisticModelSerializer probabilisticModelSerializer;

public:

    explicit ProbabilisticGenerator(ProbabilisticModelSerializer probabilisticModelSerializer);

    void generateTextInteractively(int generationSize) override;
    int getModelWindowSize() override;

    char generateNextCharacter(const std::string& currentWindow, bool useFuturePredictionAlgorithm);
    static char getCharacterUsingUniformDist(const std::map<char, double>& charactersProbabilityDistribution);
    bool isCharacterSafeChoice(const std::string& currentWindow,  const std::map<char, double>& charactersProbabilityDistribution,
                               int currentIteration, int maxIterations);

};


#endif
