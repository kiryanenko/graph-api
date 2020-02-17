//
// Created by a.kiryanenko on 2/16/20.
//

#ifndef GRAPH_API_SPUULTRAGRAPHADAPTER_H
#define GRAPH_API_SPUULTRAGRAPHADAPTER_H

#include <boost/property_map/property_map.hpp>
#include <boost/graph/properties.hpp>
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
    inline SpuUltraGraph::degree_size_type out_degree(SpuUltraGraph::vertex_descriptor v, const SpuUltraGraph &g) { return g.out_degree(v); }
    inline SpuUltraGraph::degree_size_type in_degree(SpuUltraGraph::vertex_descriptor v, const SpuUltraGraph &g) { return g.in_degree(v); }

    inline SpuUltraGraph::vertex_descriptor source(SpuUltraGraph::edge_descriptor e, const SpuUltraGraph &g) { return g.source(e); }
    inline SpuUltraGraph::vertex_descriptor target(SpuUltraGraph::edge_descriptor e, const SpuUltraGraph &g) { return g.target(e); }


    inline std::pair<SpuUltraGraph::vertex_iterator, SpuUltraGraph::vertex_iterator> vertices(const SpuUltraGraph &g) {
        auto vertices = g.vertices();
        return {vertices.begin(), vertices.end()};
    }

    inline std::pair<SpuUltraGraph::edge_iterator, SpuUltraGraph::edge_iterator> edges(const SpuUltraGraph &g) {
        auto edges = g.edges();
        return {edges.begin(), edges.end()};
    }

    inline std::pair<SpuUltraGraph::out_edge_iterator, SpuUltraGraph::out_edge_iterator> out_edges(SpuUltraGraph::vertex_descriptor v, const SpuUltraGraph &g) {
        auto edges = g.out_edges(v);
        return {edges.begin(), edges.end()};
    }

    inline std::pair<SpuUltraGraph::in_edge_iterator, SpuUltraGraph::in_edge_iterator> in_edges(SpuUltraGraph::vertex_descriptor v, const SpuUltraGraph &g) {
        auto edges = g.in_edges(v);
        return {edges.begin(), edges.end()};
    }


    //////////////////////// Свойства графа /////////////////////////////


    class spu_ug_vertex_id_map : public boost::put_get_helper<SPU_GRAPH::id_t, spu_ug_vertex_id_map>
    {
    public:
        typedef boost::readable_property_map_tag category;
        typedef SPU_GRAPH::id_t value_type;
        typedef SPU_GRAPH::id_t* reference;
        typedef SpuUltraGraph::vertex_descriptor key_type;
        long operator[](SpuUltraGraph::vertex_descriptor v) const { return v; }
    };

    inline spu_ug_vertex_id_map get(vertex_index_t, const SpuUltraGraph &g) { return {}; }
    inline spu_ug_vertex_id_map get(vertex_all_t, const SpuUltraGraph &g) { return {}; }

    template <>
    struct property_map<SpuUltraGraph, vertex_index_t> {
        typedef spu_ug_vertex_id_map type;
        typedef spu_ug_vertex_id_map const_type;
    };

    template <>
    struct property_map<SpuUltraGraph, vertex_all_t> {
        typedef spu_ug_vertex_id_map type;
        typedef spu_ug_vertex_id_map const_type;
    };


    class spu_ug_edge_id_map : public boost::put_get_helper<SPU_GRAPH::id_t, spu_ug_edge_id_map>
    {
    public:
        typedef boost::readable_property_map_tag category;
        typedef SPU_GRAPH::id_t value_type;
        typedef SPU_GRAPH::id_t* reference;
        typedef SpuUltraGraph::edge_descriptor key_type;
        long operator[](SpuUltraGraph::edge_descriptor v) const { return v; }
    };
    inline spu_ug_edge_id_map get(edge_index_t, const SpuUltraGraph &g) { return {}; }
    inline spu_ug_edge_id_map get(edge_all_t, const SpuUltraGraph &g) { return {}; }

    template <>
    struct property_map<SpuUltraGraph, edge_index_t> {
        typedef spu_ug_edge_id_map type;
        typedef spu_ug_edge_id_map const_type;
    };
    template <>
    struct property_map<SpuUltraGraph, edge_all_t> {
        typedef spu_ug_edge_id_map type;
        typedef spu_ug_edge_id_map const_type;
    };
}


#endif //GRAPH_API_SPUULTRAGRAPHADAPTER_H
