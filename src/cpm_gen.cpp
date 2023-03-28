#include <iostream>

#include "input/classes/GeneratorInputArguments.h"
#include "input/InputUtils.h"
#include "serialization/handler/ModelSerializerHandler.h"
#include "serialization/implementation/ProbabilisticModelSerializer.h"
#include "serialization/implementation/PositionalModelSerializer.h"
#include "reader/RandomAccessReader.h"
#include "generator/implementation/ProbabilisticGenerator.h"
#include "generator/implementation/PositionalGenerator.h"

int main(int argc, char *argv[]) {

    GeneratorInputArguments generatorInputArguments = getGeneratorInputArguments(argc, argv);

    cout << "Model File Path: " << generatorInputArguments.getModelPath() << endl;
    cout << "Amount of Characters to Generate: " << generatorInputArguments.getAmountOfCharactersToGenerate() << endl;

    ModelType modelType = ModelSerializerHandler::getModelType(generatorInputArguments.getModelPath());

    if (modelType == ModelType::POSITIONAL) {

        cout << "[!] Positional Model Detected!!!" << endl;

        PositionalModelSerializer sequentialModelSerializer = PositionalModelSerializer(generatorInputArguments.getModelPath());
        sequentialModelSerializer.loadModel();

        PositionalGenerator positionalGenerator = PositionalGenerator(sequentialModelSerializer);

        positionalGenerator.printHeader();
        positionalGenerator.generateTextInteractively(generatorInputArguments.getAmountOfCharactersToGenerate());

    } else if (modelType == ModelType::PROBABILISTIC) {

        cout << "Model Type: Probabilistic" << endl;

        ProbabilisticModelSerializer probabilisticModelSerializer = ProbabilisticModelSerializer(generatorInputArguments.getModelPath());
        probabilisticModelSerializer.loadModel();

        ProbabilisticGenerator probabilisticGenerator = ProbabilisticGenerator(probabilisticModelSerializer);

        probabilisticGenerator.printHeader();
        probabilisticGenerator.generateTextInteractively(generatorInputArguments.getAmountOfCharactersToGenerate());

    } else {
        cout << "Model Type: Unknown" << endl;
    }

    return 0;

}