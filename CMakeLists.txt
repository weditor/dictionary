PROJECT(Dictionary)
cmake_minimum_required(VERSION 3.5)

SET(CMAKE_CXX_STANDARD 14)
SET(BUILD_SHARED_LIBS TRUE)

include_directories(include)
add_library(dictionary src/dictionaryInterface.cpp  /usr/local/lib/co/libco.a)
target_link_libraries(dictionary )

add_executable(main src/main.cpp src/dictionaryInterface.cpp)
target_link_libraries(main dictionary  /usr/local/lib/co/libco.a)
