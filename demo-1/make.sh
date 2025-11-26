clear
reset
# 
rm -rf build
cmake -S . -B build  -DCMAKE_BUILD_TYPE=Debug -DCMAKE_PREFIX_PATH=/home/kevin/Dev/tools/libtorch
# 
cd build
make -j10
# 
cd ../bin
./libtorch_trace
