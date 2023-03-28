#ifndef SRC_POSITIONALMODELSERIALIZER_H
#define SRC_POSITIONALMODELSERIALIZER_H

#include <map>
#include <utility>
#include <vector>
#include "../abstract/AbstractModelSerializer.h"

class PositionalModelSerializer : public AbstractModelSerializer {

    std::map<std::string, std::vector<int>> model;
    int positionsPerSequenceLimit = 20;

    public:

        explicit PositionalModelSerializer(std::string modelPath)
            : AbstractModelSerializer(std::move(modelPath)) {};

        bool outputModel() override;
        bool loadModel() override;

        std::map<std::string, std::vector<int>> getModel();
        void setModel(std::map<std::string, std::vector<int>> model);

};


#endif
