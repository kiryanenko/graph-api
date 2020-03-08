//
// Created by a.kiryanenko on 3/8/20.
//

#ifndef GRAPH_API_SPUULTRAGRAPHPROPERTY_H
#define GRAPH_API_SPUULTRAGRAPHPROPERTY_H

#include <boost/property_map/property_map.hpp>
#include <boost/graph/properties.hpp>
#include "SpuUltraGraph.h"

namespace boost {
    using namespace SPU_GRAPH;


    template<typename PROPERTY, class VALUE_T>
    associative_property_map< std::unordered_map<SPU_GRAPH::id_t, VALUE_T> > get(PROPERTY, const SpuUltraGraph &g) {
        static std::unordered_map<
                SPU_GRAPH::id_t,
                associative_property_map< std::unordered_map<SPU_GRAPH::id_t, VALUE_T> >> property_maps;
        try {
            return property_maps[g.get_global_id()];
        } catch (out_of_range &e) {
            property_maps[g.get_global_id()] = {};
            return property_maps[g.get_global_id()];
        }
    }



    class spu_ug_vertex_id_map : public boost::put_get_helper<SPU_GRAPH::id_t, spu_ug_vertex_id_map>
    {
    public:
        typedef boost::readable_property_map_tag category;
        typedef SPU_GRAPH::id_t value_type;
        typedef SPU_GRAPH::id_t reference;
        typedef SpuUltraGraph::vertex_descriptor key_type;
        SPU_GRAPH::id_t operator[](SpuUltraGraph::vertex_descriptor v) const { return v; }
    };
    inline spu_ug_vertex_id_map get(vertex_index_t, const SpuUltraGraph &g) { return {}; }

    template <>
    struct property_map<SpuUltraGraph, vertex_index_t> {
        typedef spu_ug_vertex_id_map type;
        typedef spu_ug_vertex_id_map const_type;
    };


    class spu_ug_edge_id_map : public boost::put_get_helper<SPU_GRAPH::id_t, spu_ug_edge_id_map>
    {
    public:
        typedef boost::readable_property_map_tag category;
        typedef SPU_GRAPH::id_t value_type;
        typedef SPU_GRAPH::id_t reference;
        typedef SpuUltraGraph::edge_descriptor key_type;
        SPU_GRAPH::id_t operator[](SpuUltraGraph::edge_descriptor v) const { return v; }
    };
    inline spu_ug_edge_id_map get(edge_index_t, const SpuUltraGraph &g) { return {}; }

    template <>
    struct property_map<SpuUltraGraph, edge_index_t> {
        typedef spu_ug_edge_id_map type;
        typedef spu_ug_edge_id_map const_type;
    };


    class spu_ug_vertex_data_map_const
    {
        const SpuUltraGraph *_g;
    public:
        typedef boost::readable_property_map_tag category;
        typedef pair<SpuUltraGraph::vertex_descriptor, value_t> value_type;
        typedef pair<SpuUltraGraph::vertex_descriptor, value_t> reference;
        typedef SpuUltraGraph::vertex_descriptor key_type;

        spu_ug_vertex_data_map_const(const SpuUltraGraph *g) : _g(g) {}
        pair<SpuUltraGraph::vertex_descriptor, value_t> get(SpuUltraGraph::vertex_descriptor v) const {
            return {v, _g->get_vertex_value(v)};
        }
    };
    inline spu_ug_vertex_data_map_const get(vertex_all_t, const SpuUltraGraph &g) { return {&g}; }
    inline pair<SpuUltraGraph::vertex_descriptor, value_t> get(const spu_ug_vertex_data_map_const &pm, SpuUltraGraph::vertex_descriptor v) { return pm.get(v); }

    class spu_ug_vertex_data_map : public spu_ug_vertex_data_map_const
    {
        SpuUltraGraph *_g;
    public:
        typedef boost::read_write_property_map_tag category;

        spu_ug_vertex_data_map(SpuUltraGraph *g) : spu_ug_vertex_data_map_const(g),  _g(g) {}
        void put(SpuUltraGraph::vertex_descriptor v, value_t value) {
            _g->put_vertex(v, value);
        }
    };
    inline spu_ug_vertex_data_map get(vertex_all_t, SpuUltraGraph &g) { return {&g}; }
    inline void put(spu_ug_vertex_data_map &pm, SpuUltraGraph::vertex_descriptor v, value_t value) { return pm.put(v, value); }

    template <>
    struct property_map<SpuUltraGraph, vertex_all_t> {
        typedef spu_ug_vertex_data_map type;
        typedef spu_ug_vertex_data_map_const const_type;
    };


    class spu_ug_edge_data_map_const
    {
        const SpuUltraGraph *_g;
    public:
        typedef boost::readable_property_map_tag category;
        typedef pair<SpuUltraGraph::edge_descriptor, value_t> value_type;
        typedef pair<SpuUltraGraph::edge_descriptor, value_t> reference;
        typedef SpuUltraGraph::edge_descriptor key_type;

        spu_ug_edge_data_map_const(const SpuUltraGraph *g) : _g(g) {}
        pair<SpuUltraGraph::edge_descriptor, value_t> get(SpuUltraGraph::edge_descriptor e) const {
            return {e, _g->get_edge_value(e)};
        }
    };
    inline spu_ug_edge_data_map_const get(edge_all_t, const SpuUltraGraph &g) { return {&g}; }
    inline pair<SpuUltraGraph::edge_descriptor, value_t> get(const spu_ug_edge_data_map_const &pm, SpuUltraGraph::edge_descriptor e) { return pm.get(e); }

    class spu_ug_edge_data_map : public spu_ug_edge_data_map_const
    {
        SpuUltraGraph *_g;
    public:
        typedef boost::read_write_property_map_tag category;

        spu_ug_edge_data_map(SpuUltraGraph *g) : spu_ug_edge_data_map_const(g),  _g(g) {}
        void put(SpuUltraGraph::edge_descriptor e, value_t value) {
            _g->put_edge(e, value);
        }
    };
    inline spu_ug_edge_data_map get(edge_all_t, SpuUltraGraph &g) { return {&g}; }
    inline void put(spu_ug_edge_data_map &pm, SpuUltraGraph::edge_descriptor e, value_t value) { return pm.put(e, value); }

    template <>
    struct property_map<SpuUltraGraph, edge_all_t> {
        typedef spu_ug_edge_data_map type;
        typedef spu_ug_edge_data_map_const const_type;
    };
}

#endif //GRAPH_API_SPUULTRAGRAPHPROPERTY_H
