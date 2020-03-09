//
// Created by a.kiryanenko on 2/16/20.
//

#ifndef GRAPH_API_SPUULTRAGRAPHADAPTER_H
#define GRAPH_API_SPUULTRAGRAPHADAPTER_H

#include "SpuUltraGraph.h"

namespace boost {
    using namespace SPU_GRAPH;


    inline SpuUltraGraph::vertex_descriptor add_vertex(SpuUltraGraph &g) { return g.add_vertex(); }

    inline std::pair<SpuUltraGraph::edge_descriptor, bool>
    add_edge(SpuUltraGraph::vertex_descriptor from, SpuUltraGraph::vertex_descriptor to, SpuUltraGraph &g) {
        return {g.add_edge(from, to), true};
    }

    inline void remove_edge(SpuUltraGraph::vertex_descriptor u, SpuUltraGraph::vertex_descriptor v, SpuUltraGraph &g) { g.remove_edge(u, v); }
    inline void remove_edge(SpuUltraGraph::edge_descriptor edge, SpuUltraGraph &g) { g.remove_edge(edge); }
    inline void clear_vertex(SpuUltraGraph::vertex_descriptor v, SpuUltraGraph &g) { g.clear_vertex(v);}
    inline void remove_vertex(SpuUltraGraph::vertex_descriptor v, SpuUltraGraph &g) { g.remove_vertex(v); }

    inline SpuUltraGraph::vertices_size_type num_vertices(const SpuUltraGraph &g) { return g.num_vertices(); }
    inline SpuUltraGraph::edges_size_type num_edges(const SpuUltraGraph &g) { return g.num_edges(); }
    inline SpuUltraGraph::degree_size_type degree(SpuUltraGraph::vertex_descriptor v, const SpuUltraGraph &g) { return g.degree(v); }
    inline SpuUltraGraph::degree_size_type out_degree(SpuUltraGraph::vertex_descriptor v, const SpuUltraGraph &g) { return g.out_degree(v); }
    inline SpuUltraGraph::degree_size_type in_degree(SpuUltraGraph::vertex_descriptor v, const SpuUltraGraph &g) { return g.in_degree(v); }

    inline SpuUltraGraph::vertex_descriptor source(SpuUltraGraph::edge_descriptor e, const SpuUltraGraph &g) { return g.source(e); }
    inline SpuUltraGraph::vertex_descriptor target(SpuUltraGraph::edge_descriptor e, const SpuUltraGraph &g) { return g.target(e); }

    inline pair<SpuUltraGraph::edge_descriptor, bool>
    edge(SpuUltraGraph::vertex_descriptor u, SpuUltraGraph::vertex_descriptor v, const SpuUltraGraph &g) {
        auto e = g.edge(u, v);
        return {e, bool(e)};
    }


    inline std::pair<SpuUltraGraph::vertex_iterator, SpuUltraGraph::vertex_iterator> vertices(const SpuUltraGraph &g) {
        auto vertices = g.vertices();
        return {vertices.begin(), vertices.end()};
    }

    inline std::pair<SpuUltraGraph::adjacency_iterator, SpuUltraGraph::adjacency_iterator>
    adjacent_vertices(SpuUltraGraph::vertex_descriptor v, const SpuUltraGraph &g) {
        auto vertices = g.adjacent_vertices(v);
        return {vertices.begin(), vertices.end()};
    }

    inline std::pair<SpuUltraGraph::edge_iterator, SpuUltraGraph::edge_iterator> edges(const SpuUltraGraph &g) {
        auto edges = g.edges();
        return {edges.begin(), edges.end()};
    }

    inline std::pair<SpuUltraGraph::out_edge_iterator, SpuUltraGraph::out_edge_iterator>
    out_edges(SpuUltraGraph::vertex_descriptor v, const SpuUltraGraph &g) {
        auto edges = g.out_edges(v);
        return {edges.begin(), edges.end()};
    }

    inline std::pair<SpuUltraGraph::in_edge_iterator, SpuUltraGraph::in_edge_iterator>
    in_edges(SpuUltraGraph::vertex_descriptor v, const SpuUltraGraph &g) {
        auto edges = g.in_edges(v);
        return {edges.begin(), edges.end()};
    }
}


#endif //GRAPH_API_SPUULTRAGRAPHADAPTER_H
