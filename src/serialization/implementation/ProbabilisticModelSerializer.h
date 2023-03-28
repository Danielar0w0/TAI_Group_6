#ifndef SRC_PROBABILISTICMODELSERIALIZER_H
#define SRC_PROBABILISTICMODELSERIALIZER_H

#include <map>
#include <string>
#include "../abstract/AbstractModelSerializer.h"

class ProbabilisticModelSerializer : public AbstractModelSerializer {

    std::map<std::string, std::map<char, double>> model;

    public:

        explicit ProbabilisticModelSerializer(std::string modelPath)
            : AbstractModelSerializer(std::move(modelPath)) {};

        bool outputModel() override;
        bool loadModel() override;

        std::map<std::string, std::map<char, double>> getModel();
        void setModel(std::map<std::string, std::map<char, double>> model);

};


#endif
