#ifndef SRC_ABSTRACTMODELSERIALIZER_H
#define SRC_ABSTRACTMODELSERIALIZER_H


#include <string>
#include <map>


class AbstractModelSerializer {

    protected:

        std::string modelPath;
        std::string inputFilePath;
        explicit AbstractModelSerializer(std::string modelPath);


    public:

        virtual ~AbstractModelSerializer() = default;

        virtual bool outputModel() = 0;
        virtual bool loadModel() = 0;

        std::string getModelPath();
        std::string getInputFilePath();
        void setInputFilePath(const std::string& inputPath);

};

#endif
