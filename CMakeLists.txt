cmake_minimum_required(VERSION 3.16)
project(BrnRenderer LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(rapidobj
    GIT_REPOSITORY https://github.com/guybrush77/rapidobj.git
    GIT_TAG v1.1)
FetchContent_MakeAvailable(rapidobj)

include_directories(include/)
include_directories(${rapidobj_SOURCE_DIR}/include)

file(GLOB_RECURSE SRC_FILES src/*.cpp)

add_executable(BrnRenderer ${SRC_FILES})
target_link_libraries(BrnRenderer PRIVATE sfml-graphics)
target_compile_features(BrnRenderer PRIVATE cxx_std_17)
