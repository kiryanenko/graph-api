//
// Created by a.kiryanenko on 3/26/20.
//

#ifndef GRAPH_API_UTILS_H
#define GRAPH_API_UTILS_H

#include <iostream>
#include <vector>
#include <boost/graph/graph_traits.hpp>


using namespace std;
using namespace boost;


//template<class G>
//typename G::vertex_descriptor add_edge(typename G::vertex_descriptor, typename G::vertex_descriptor, G&);

template<class G>
struct fill_options
{
    typedef typename graph_traits<G>::vertex_descriptor vertex_t;
    typedef typename graph_traits<G>::edge_descriptor edge_t;

    unsigned long rand_init = 0;
    pair<edge_t, bool> (*add_edge_func)(vertex_t, vertex_t, G&) = add_edge;
};

template<class G>
void fill_graph(G &g, size_t vertices_cnt, size_t edges_cnt, fill_options<G> opts = {})
{
    srand(opts.rand_init);

    vector<typename G::vertex_descriptor> vertices(vertices_cnt);
    for (size_t i = 0; i < vertices_cnt; ++i) {
        auto v = add_vertex(g);
        vertices[i] = v;
        if (i > 1) {
            auto u = vertices[rand() % i];
            opts.add_edge_func(u, v, g);
        }
    }

    for (size_t i = 0; i < edges_cnt - vertices_cnt; ++i) {
        auto u = vertices[rand() % vertices_cnt];
        auto v = vertices[rand() % vertices_cnt];
        opts.add_edge_func(u, v, g);
    }
}

#endif //GRAPH_API_UTILS_H
