mkdir bin
cd bin
cmake -DCMAKE_BUILD_TYPE=Release ../src
make
find . -mindepth 1 ! -name "cpm_gen" ! -name "cpm" -exec rm -rf {} +
