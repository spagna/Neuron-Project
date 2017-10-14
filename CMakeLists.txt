cmake_minimum_required (VERSION 2.6)
project (NeuronProject)

set(INCLUDE_DIRECTORY inc)
set(CMAKE_CXX_FLAGS "-O3 -W -Wall -pedantic -std=c++11")

# Finds .h/.hpp files
include_directories("${INCLUDE_DIRECTORY}")

add_executable (${PROJECT_NAME} src/Neuron.cpp src/main.cpp)

