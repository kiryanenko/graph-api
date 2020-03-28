//
// Created by a.kiryanenko on 3/29/20.
//

#ifndef GRAPH_API_GRAPHPERFORMANCETEST_H
#define GRAPH_API_GRAPHPERFORMANCETEST_H

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

    size_t start_edges_cnt = 300;
    size_t inc_edges_value = 300;
    size_t end_edges_cnt = 30000;

    size_t avg_iterations_cnt = 10;
    string results_file = "results.csv";

    void start();
};


#endif //GRAPH_API_GRAPHPERFORMANCETEST_H
