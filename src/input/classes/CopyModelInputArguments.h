#ifndef SRC_COPYMODELINPUTARGUMENTS_H
#define SRC_COPYMODELINPUTARGUMENTS_H

#include <string>
#include <iostream>

#include "../../utils/generalUtils.h"

class CopyModelInputArguments {

    double alpha{};
    int k{};
    double threshold{};
    std::string inputFilePath;
    std::string outputModelPath;
    int modelBuilderType{};

    public:

        CopyModelInputArguments();
        CopyModelInputArguments(std::string inputFilePath, std::string outputFilePath, double alpha, int k, int modelBuilderType);

        [[nodiscard]] double getAlpha() const;
        [[nodiscard]] int getK() const;
        [[nodiscard]] double getThreshold() const;
        [[nodiscard]] std::string getInputFilePath() const;
        [[nodiscard]] std::string getOutputModelPath() const;
        [[nodiscard]] int getModelBuilderType() const;

        void parseArguments(int argc, char *argv[]);
        // Don't add nodiscard - I may want to check the arguments and do nothing with the return value.
        bool checkArguments() const;

        static void printUsage();

};


#endif
