clear
reset
# 
# -DCMAKE_PREFIX_PATH=/home/kevin/Dev/tools/libtorch 
rm -rf build
cmake -S . -B build  -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=/home/kevin/Dev/tools/vcpkg/scripts/buildsystems/vcpkg.cmake
# 
cd build
make -j10
# 
cd ../bin
./demo
