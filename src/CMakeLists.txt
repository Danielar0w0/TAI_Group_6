cmake_minimum_required(VERSION 3.24)
project(src)

set(CMAKE_CXX_STANDARD 17)

add_executable(src cmp.cpp utils/printUtils.h input/classes/InputArguments.cpp input/classes/InputArguments.h utils/readTarget.h input/InputUtils.h utils/utils.h reader/FileReader.cpp reader/FileReader.h reader/FileInfo.cpp reader/FileInfo.h cmp_gen.cpp hitsMisses/HitsMissesInfo.cpp hitsMisses/HitsMissesInfo.h modelSerializer/ModelSerializer.cpp modelSerializer/ModelSerializer.h)
