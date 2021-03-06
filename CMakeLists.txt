cmake_minimum_required(VERSION 2.6)
project(AutoParts)

set(USE_MPI ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -msse -msse2 -msse3 -stdlib=libstdc++")

# Add extra CMake libraries into ./CMake
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake ${CMAKE_MODULE_PATH})

# Set source root relate to project file
set(PROJECT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})


############# MPI #################

if(USE_MPI)
  # then section.
  add_definitions(-DAP_MPI)
  # Require MPI for this project:
  find_package(MPI REQUIRED)
  include_directories(${MPI_INCLUDE_PATH})
  set(CMAKE_CXX_COMPILE_FLAGS ${CMAKE_CXX_COMPILE_FLAGS} ${MPI_COMPILE_FLAGS})
  set(CMAKE_CXX_LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS} ${MPI_LINK_FLAGS})
endif()



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

if(USE_MPI)
  # then section.
  target_link_libraries(AutoParts ${MPI_LIBRARIES})
else()
  # else section.
  target_link_libraries(AutoParts)
endif()



