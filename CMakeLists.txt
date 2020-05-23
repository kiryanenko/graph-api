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
        SpuUltraGraph.cpp SpuUltraGraph.h
        GraphStructure.h StructureIterator.h SpuUltraGraphAdapter.h SpuUltraGraphProperty.h UltraGraphConcepts.h
        utils.h utils.cpp)

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
add_executable(graph_api_example examples/graph_api_example.cpp)
target_link_libraries(graph_api_example graph_api ${Boost_LIBRARIES})

add_executable(graphvis_example examples/graphvis_example.cpp)
target_link_libraries(graphvis_example graph_api ${Boost_LIBRARIES})

add_executable(dijkstra_example examples/dijkstra_example.cpp)
target_link_libraries(dijkstra_example graph_api ${Boost_LIBRARIES})

# Тесты производительности
file(GLOB_RECURSE performance_test_sources performance_tests/utils.h performance_tests/GraphPerformanceTest.h)

add_executable(dijkstra_perf performance_tests/dijkstra.cpp ${performance_test_sources})
target_link_libraries(dijkstra_perf graph_api ${Boost_LIBRARIES})

add_executable(kruskal_perf performance_tests/kruskal.cpp ${performance_test_sources})
target_link_libraries(kruskal_perf graph_api ${Boost_LIBRARIES})

add_executable(depth_first_perf performance_tests/depth_first.cpp ${performance_test_sources})
target_link_libraries(depth_first_perf graph_api ${Boost_LIBRARIES})
