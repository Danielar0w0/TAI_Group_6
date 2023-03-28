#ifndef SRC_PROBABILISTICGENERATOR_H
#define SRC_PROBABILISTICGENERATOR_H

#include "../abstract/AbstractGenerator.h"
#include "../../serialization/implementation/ProbabilisticModelSerializer.h"

class ProbabilisticGenerator : public AbstractGenerator {

    ProbabilisticModelSerializer probabilisticModelSerializer;
    bool useOptimization = false;
    int optimizationAggressiveness = 1;

public:

    explicit ProbabilisticGenerator(ProbabilisticModelSerializer probabilisticModelSerializer);

    void generateTextInteractively(int generationSize) override;
    void generateTextOnce(int generationSize) override;
    int getModelWindowSize() override;

    void setUseOptimization(bool useOptimization);
    void setOptimizationAggressiveness(int optimizationAggressiveness);

private:

    std::string generateText(int generationSize, const std::string& initialText, const std::map<std::string, std::map<char, double>>& model, bool useFuturePredictionAlgorithm);
    char generateNextCharacter(const std::string& currentWindow, bool useFuturePredictionAlgorithm);
    static char getCharacterUsingUniformDist(const std::map<char, double>& charactersProbabilityDistribution);
    bool isCharacterSafeChoice(const std::string& currentWindow,  const std::map<char, double>& charactersProbabilityDistribution,
                               int currentIteration, int maxIterations);

};


#endif
