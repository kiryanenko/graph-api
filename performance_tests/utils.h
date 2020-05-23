//
// Created by a.kiryanenko on 3/26/20.
//

#ifndef GRAPH_API_UTILS_H
#define GRAPH_API_UTILS_H

#include <iostream>
#include <vector>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_matrix.hpp>


using namespace std;
using namespace boost;


typedef boost::adjacency_matrix <
        boost::directedS,
        no_property,
        property < edge_weight_t, int >
> AdjacencyMatrixGraph;


template<class G>
struct fill_options
{
    typedef typename graph_traits<G>::vertex_descriptor vertex_t;
    typedef typename graph_traits<G>::edge_descriptor edge_t;

    unsigned long rand_init = 0;
    vertex_t (*add_vertex_func)(G&) = nullptr;
    pair<edge_t, bool> (*add_edge_func)(vertex_t, vertex_t, G&) = nullptr;
};


template<class G>
void fill_random_graph(G &g, size_t vertices_cnt, size_t edges_cnt, fill_options<G> opts = {})
{
    srand(opts.rand_init);

    vector<typename G::vertex_descriptor> vertices(vertices_cnt);
    for (size_t i = 0; i < vertices_cnt; ++i) {
        auto v = opts.add_vertex_func ? opts.add_vertex_func(g) : add_vertex(g);
        vertices[i] = v;
        if (i > 1) {
            auto u = vertices[rand() % i];
            opts.add_edge_func ? opts.add_edge_func(u, v, g) : add_edge(u, v, g);
        }
    }

    for (size_t i = 0; i < edges_cnt - vertices_cnt; ++i) {
        auto u = vertices[rand() % vertices_cnt];
        auto v = vertices[rand() % vertices_cnt];
        opts.add_edge_func ? opts.add_edge_func(u, v, g) : add_edge(u, v, g);
    }
}


template<class G>
void fill_grid_graph(G &g, size_t vertices_cnt, size_t edges_cnt, fill_options<G> opts = {})
{
    srand(opts.rand_init);

    size_t edges_per_vertex = ceil((vertices_cnt + 0.0) / edges_cnt);
    size_t cell_width = ceil(sqrt(vertices_cnt));
    vector<typename G::vertex_descriptor> vertices(vertices_cnt);
    for (size_t i = 0; i < vertices_cnt; ++i) {
        auto v = opts.add_vertex_func ? opts.add_vertex_func(g) : add_vertex(g);
        vertices[i] = v;

        size_t row = i / cell_width;
        size_t col = i % cell_width;
        for (size_t edge = 0; edge < edges_per_vertex; ++edge) {
            size_t edge_type = edge % 2;
            if (i * edges_per_vertex + edge > edges_cnt) {
                break;
            } else if (edge_type == 0 && col > 0) {
                auto u = vertices[row * cell_width + col - 1];
                opts.add_edge_func ? opts.add_edge_func(u, v, g) : add_edge(u, v, g);
            } else if (edge_type == 1 && row > 0) {
                auto u = vertices[(row - 1) * cell_width + col];
                opts.add_edge_func ? opts.add_edge_func(u, v, g) : add_edge(u, v, g);
            }
        }
    }
}

template<>
void fill_grid_graph(AdjacencyMatrixGraph &g, size_t vertices_cnt, size_t edges_cnt, fill_options<AdjacencyMatrixGraph> opts)
{
    srand(opts.rand_init);

    size_t edges_per_vertex = ceil((vertices_cnt + 0.0) / edges_cnt);
    size_t cell_width = ceil(sqrt(vertices_cnt));
    for (size_t v = 0; v < vertices_cnt; ++v) {
        size_t row = v / cell_width;
        size_t col = v % cell_width;
        for (size_t edge = 0; edge < edges_per_vertex; ++edge) {
            size_t edge_type = edge % 2;
            if (v * edges_per_vertex + edge > edges_cnt) {
                break;
            } else if (edge_type == 0 && col > 0) {
                auto u = row * cell_width + col - 1;
                opts.add_edge_func ? opts.add_edge_func(u, v, g) : add_edge(u, v, g);
            } else if (edge_type == 1 && row > 0) {
                auto u = (row - 1) * cell_width + col;
                opts.add_edge_func ? opts.add_edge_func(u, v, g) : add_edge(u, v, g);
            }
        }
    }
}


#endif //GRAPH_API_UTILS_H
