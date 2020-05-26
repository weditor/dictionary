PROJECT(Dictionary)
cmake_minimum_required(VERSION 3.5)

SET(CMAKE_CXX_STANDARD 14)
SET(BUILD_SHARED_LIBS TRUE)

FIND_LIBRARY(LIBCO libco.a HINTS /usr/local/lib/co)

message(use LIBCO: ${LIBCO})

include_directories(include)
add_library(dictionary src/dictionaryInterface.cpp src/dawgInterface.cpp )
target_link_libraries(dictionary ${LIBCO})

add_executable(main src/main.cpp)
target_link_libraries(main dictionary)
