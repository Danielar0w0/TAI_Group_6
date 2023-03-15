cmake_minimum_required(VERSION 3.24)
project(src)

set(CMAKE_CXX_STANDARD 17)

add_executable(cpm cpm.cpp utils/printUtils.h input/classes/CopyModelInputArguments.cpp input/classes/CopyModelInputArguments.h utils/readTarget.h input/InputUtils.h utils/utils.h reader/FileReader.cpp reader/FileReader.h reader/FileInfo.cpp reader/FileInfo.h hitsMisses/HitsMissesInfo.cpp hitsMisses/HitsMissesInfo.h modelSerializer/ModelSerializer.cpp modelSerializer/ModelSerializer.h input/classes/GeneratorInputArguments.cpp input/classes/GeneratorInputArguments.h)

add_executable(gen cpm_gen.cpp utils/printUtils.h input/classes/CopyModelInputArguments.cpp input/classes/CopyModelInputArguments.h utils/readTarget.h input/InputUtils.h utils/utils.h reader/FileReader.cpp reader/FileReader.h reader/FileInfo.cpp reader/FileInfo.h hitsMisses/HitsMissesInfo.cpp hitsMisses/HitsMissesInfo.h modelSerializer/ModelSerializer.cpp modelSerializer/ModelSerializer.h input/classes/GeneratorInputArguments.cpp input/classes/GeneratorInputArguments.h)