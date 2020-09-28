cmake_minimum_required(VERSION 3.17)
project(SIMULATOR)
aux_source_directory(./source/cpp/ DIRSRCS)
set(CMAKE_CXX_STANDARD 14)

add_subdirectory(source/cpp)

add_executable(simulator ${DIRSRCS})
target_link_libraries(simulator ds emitter propagation type)
