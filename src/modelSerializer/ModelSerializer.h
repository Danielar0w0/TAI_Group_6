#ifndef SRC_MODELSERIALIZER_H
#define SRC_MODELSERIALIZER_H

#include <map>
#include <string>

class ModelSerializer {
    std::map<std::string, std::map<char, double>> model;

    public:

        explicit ModelSerializer(std::map<std::string, std::map<char, double>> model);
        explicit ModelSerializer();

        int outputModel(const std::string& outputPath);
        int loadModel(const std::string& inputPath);
        void printModel(int lim);
        // static std::map<std::string, std::map<char, double>> getModel();



};


#endif //SRC_MODELSERIALIZER_H
