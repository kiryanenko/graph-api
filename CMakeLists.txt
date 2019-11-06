cmake_minimum_required(VERSION 3.14)
project(graph_api)

set(CMAKE_CXX_STANDARD 14)

add_library(graph_api
        graphs/graph/Graph.h graphs/graph/Graph.cpp
        graphs/graph/Node.h graphs/graph/Node.cpp
        graphs/graph/Edge.h graphs/graph/Edge.cpp
        graphs/exceptions.h
        graph/types.h
        SpuUltraGraph.cpp SpuUltraGraph.h)


set(SOURCE_EXE main.cpp graph/Graph.cpp graphs/graph/Node.cpp graphs/graph/Node.h graphs/graph/GraphApi.cpp graphs/graph/GraphApi.h)
add_executable(main ${SOURCE_EXE})
target_link_libraries(main graph_api)		# Линковка программы с библиотекой