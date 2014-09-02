cmake_minimum_required(VERSION 2.6)
project(AutoParts)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -msse -msse2 -msse3 -stdlib=libstdc++")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -Wall")

# Add extra CMake libraries into ./CMake
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake ${CMAKE_MODULE_PATH})

# Set source root relate to project file
set(PROJECT_SOURCE_DIR ${CMAKE_SOURCE_DIR})

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})


############# executables #################
# basic rev-bayes binary
add_executable(AutoParts 
    ${PROJECT_SOURCE_DIR}/main.cpp 
    ${PROJECT_SOURCE_DIR}/Alignment.cpp
    ${PROJECT_SOURCE_DIR}/Chunk.cpp
    ${PROJECT_SOURCE_DIR}/MbEigensystem.cpp
    ${PROJECT_SOURCE_DIR}/MbMath.cpp
    ${PROJECT_SOURCE_DIR}/MbRandom.cpp
    ${PROJECT_SOURCE_DIR}/MbTransitionMatrix.cpp
    ${PROJECT_SOURCE_DIR}/Mcmc.cpp
    ${PROJECT_SOURCE_DIR}/Model.cpp
    ${PROJECT_SOURCE_DIR}/Parm.cpp
    ${PROJECT_SOURCE_DIR}/ParmAsrv.cpp
    ${PROJECT_SOURCE_DIR}/ParmFreqs.cpp
    ${PROJECT_SOURCE_DIR}/ParmLength.cpp
    ${PROJECT_SOURCE_DIR}/ParmSubrates.cpp
    ${PROJECT_SOURCE_DIR}/ParmTree.cpp
    ${PROJECT_SOURCE_DIR}/Restaurant.cpp
    ${PROJECT_SOURCE_DIR}/Settings.cpp
    ${PROJECT_SOURCE_DIR}/StateSets.cpp
    ${PROJECT_SOURCE_DIR}/Table.cpp
)

target_link_libraries(AutoParts)


