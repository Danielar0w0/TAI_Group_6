#ifndef SRC_COPYMODELINPUTARGUMENTS_H
#define SRC_COPYMODELINPUTARGUMENTS_H

#include <string>
#include <iostream>

#include "../../utils/generalUtils.h"
#include "../abstract/AbstractInputArguments.h"

class CopyModelInputArguments : public AbstractInputArguments {

    double alpha = 0.5;
    int k = 3;
    double threshold = 0.4;
    std::string inputFilePath{};
    std::string outputModelPath{};
    int loggingLevel = 1;
    int serializerType{};
    bool serializeForGenerator = false;

    public:

        CopyModelInputArguments();

        [[nodiscard]] double getAlpha() const;
        [[nodiscard]] int getK() const;
        [[nodiscard]] double getThreshold() const;
        [[nodiscard]] std::string getInputFilePath() const;
        [[nodiscard]] std::string getOutputModelPath() const;
        [[nodiscard]] int getLoggingLevel() const;
        [[nodiscard]] int getSerializerType() const;
        [[nodiscard]] bool shouldSerializeForGenerator() const;

        void parseArguments(int argc, char *argv[]) override;

        // Don't add nodiscard - I may want to check the arguments and do nothing with the return value.
        bool checkArguments() const override;

        static void printUsage();

};


#endif
