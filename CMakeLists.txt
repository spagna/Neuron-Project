cmake_minimum_required (VERSION 2.6)
project (NeuronProject)

set(INCLUDE_DIRECTORY inc)
set(CMAKE_CXX_FLAGS "-O3 -W -Wall -pedantic -std=c++11")

enable_testing()
add_subdirectory(gtest)
include_directories(${gtest_SOURCE_DIR}/include${gtest_SOURCE_DIR})

# Finds .h/.hpp files
include_directories("${INCLUDE_DIRECTORY}")

add_executable (${PROJECT_NAME} src/Neuron.cpp src/main.cpp)
add_executable(Neuron_unittest src/Neuron.cpp src/Neuron_unittest.cpp)

target_link_libraries(Neuron_unittest gtest gtest_main)
add_test(Neuron_unittest Neuron_unittest)
