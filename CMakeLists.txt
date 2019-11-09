cmake_minimum_required(VERSION 3.14)
project(graph_api)

set(SPU_ARCH 64)
set(SPU_SIMULATOR ON)

set(CMAKE_CXX_STANDARD 14)

if(${SPU_SIMULATOR})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSPU_SIMULATOR")
endif(${SPU_SIMULATOR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS} -Wall -ggdb -DSPU${SPU_ARCH}")


add_library(graph_api
        SpuUltraGraph.cpp SpuUltraGraph.h)

include_directories(spu-api/libspu)
add_subdirectory(spu-api)
target_link_libraries(graph_api spu-api)

set(SOURCE_EXE main.cpp)
add_executable(graph_main ${SOURCE_EXE})
target_link_libraries(graph_main graph_api)		# Линковка программы с библиотекой