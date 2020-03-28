//
// Created by a.kiryanenko on 3/29/20.
//

#ifndef GRAPH_API_GRAPHPERFORMANCETEST_H
#define GRAPH_API_GRAPHPERFORMANCETEST_H

#include <utility>

#include "utils.h"


template <class G>
class GraphPerformanceTest {
    typedef typename graph_traits<G>::vertex_descriptor vertex_t;
    typedef typename graph_traits<G>::edge_descriptor edge_t;

public:
    bool should_fill = true;
    pair<edge_t, bool> (*add_edge_func)(vertex_t, vertex_t, G&) = add_edge;

    size_t start_vertices_cnt = 100;
    size_t inc_vertices_value = 100;
    size_t end_vertices_cnt = 10000;
    size_t edges_per_vertex = 3;

    size_t avg_iterations_cnt = 10;

    void (*test_func)() = nullptr;
    string results_file = "results.csv";

    GraphPerformanceTest() = default;
    GraphPerformanceTest(void (*test_func)(), string results_file) : test_func(test_func), results_file(std::move(results_file)) {}
    void start();
};


#endif //GRAPH_API_GRAPHPERFORMANCETEST_H
