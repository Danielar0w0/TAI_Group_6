#ifndef SRC_SEQUENTIALMODELSERIALIZER_H
#define SRC_SEQUENTIALMODELSERIALIZER_H

#include <map>
#include <utility>
#include "../abstract/AbstractModelSerializer.h"

class SequentialModelSerializer : public AbstractModelSerializer {

    std::map<std::string, std::string> model;

    public:

        explicit SequentialModelSerializer(std::string modelPath)
            : AbstractModelSerializer(std::move(modelPath)) {};

        bool outputModel() override;
        bool loadModel() override;

        std::map<std::string, std::string> getModel();
        void setModel(std::map<std::string, std::string> model);

};


#endif
