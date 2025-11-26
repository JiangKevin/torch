cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(libtorch_trace)

# 需要找到Libtorch
find_package(Torch REQUIRED)
# 
# 
file(GLOB SRC_FILES 
    "src/*.cpp"
)
# 设置所有可执行文件的输出目录（相对路径，相对于构建目录；也可写绝对路径）
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
# 设置 C++ 标准为 14
set(CMAKE_CXX_STANDARD 17)
# 强制要求标准，不允许降级到更低版本（如 C++11）
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# 禁用编译器扩展（如 GCC 的 gnu++14，确保使用纯 C++14）
set(CMAKE_CXX_EXTENSIONS OFF)
# 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

add_executable(${PROJECT_NAME}
    src/libtorch_trace.cpp
)
target_link_libraries(${PROJECT_NAME} "${TORCH_LIBRARIES}")

