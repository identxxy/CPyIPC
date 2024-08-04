# CMakeLists.txt
cmake_minimum_required(VERSION 3.20)
project(binding)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED COMPONENTS interprocess)
include_directories(${Boost_INCLUDE_DIR})

include_directories(.)

add_library(c STATIC interface.cpp)
target_link_libraries(c PRIVATE Boost::interprocess)
target_compile_options(c PRIVATE -fPIC)

# If pybind11 is local
add_subdirectory(pybind11)
# If pybind11 is installed
# find_package(pybind11)
find_package(Python 3.11 COMPONENTS Interpreter Development REQUIRED)
pybind11_add_module(
    _c
    pybind.cpp
)
target_link_libraries(_c PRIVATE c Boost::interprocess)

add_executable(main examples/main.cpp)
target_link_libraries(main PRIVATE c Boost::interprocess)

add_executable(main2 examples/main2.cpp)
target_link_libraries(main2 PRIVATE c Boost::interprocess)
