cmake_minimum_required(VERSION 3.14)
project(graph_api)

set(CMAKE_CXX_STANDARD 14)

add_library(graph_api
        graph/AbstractGraph.h graph/AbstractGraph.cpp
        graph/Node.cpp graph/Node.h
        graph/Edge.cpp graph/Edge.h
        exceptions.h
        graph/types.h
        SpuUltraGraph.cpp SpuUltraGraph.h)


set(SOURCE_EXE main.cpp graph/AbstractGraph.cpp)
add_executable(main ${SOURCE_EXE})
target_link_libraries(main graph_api)		# Линковка программы с библиотекой