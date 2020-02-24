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
        SpuUltraGraph.cpp SpuUltraGraph.h GraphStructure.h StructureIterator.h SpuUltraGraphAdapter.h)

include_directories(spu-api/libspu)
add_subdirectory(spu-api)
target_link_libraries(graph_api spu-api)


# Тесты
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED OFF)
find_package (Boost REQUIRED COMPONENTS unit_test_framework REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

set(TEST test_graph_api)
file(GLOB_RECURSE graph_api_tests_sources tests/*.cpp)
add_executable(${TEST} ${graph_api_tests_sources})
target_link_libraries(${TEST} graph_api ${Boost_LIBRARIES})

enable_testing()
add_test (${TEST} ${TEST})


# Примеры
add_executable(graph_api_example examples/main.cpp)
target_link_libraries(graph_api_example graph_api ${Boost_LIBRARIES})
