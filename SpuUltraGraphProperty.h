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
    class spu_ug_readable_property_map
    {
        const SpuUltraGraph *_g;
    public:
        typedef boost::readable_property_map_tag category;
        typedef VALUE_T value_type;
        typedef VALUE_T reference;
        typedef SPU_GRAPH::id_t key_type;

        spu_ug_readable_property_map(const SpuUltraGraph *g) : _g(g) {}
        const SpuUltraGraph *get_graph() const { return _g; }
    };

    template<typename PROPERTY, class VALUE_T>
    class spu_ug_property_map : public spu_ug_readable_property_map<PROPERTY, VALUE_T>
    {
        SpuUltraGraph *_g;
    public:
        typedef boost::read_write_property_map_tag category;
        spu_ug_property_map(SpuUltraGraph *g) : spu_ug_readable_property_map<PROPERTY, VALUE_T>(g), _g(g) {}
        SpuUltraGraph *get_graph() const { return _g; }
    };



    typedef spu_ug_readable_property_map<vertex_index_t, SPU_GRAPH::id_t> spu_ug_vertex_id_pm;
    spu_ug_vertex_id_pm get(vertex_index_t, const SpuUltraGraph &g) { return {&g}; }
    SpuUltraGraph::vertex_descriptor get(const spu_ug_vertex_id_pm&, SPU_GRAPH::id_t id) { return id; }
    SpuUltraGraph::vertex_descriptor get(vertex_index_t tag, const SpuUltraGraph &g, SPU_GRAPH::id_t id) { return id; }

    template <>
    struct property_map<SpuUltraGraph, vertex_index_t> {
        typedef spu_ug_vertex_id_pm const_type;
        typedef const_type type;
    };


    typedef spu_ug_readable_property_map<edge_index_t, SpuUltraGraph::edge_descriptor> spu_ug_edge_id_pm;
    spu_ug_edge_id_pm get(edge_index_t, const SpuUltraGraph &g) { return {&g}; }
    SpuUltraGraph::edge_descriptor get(const spu_ug_edge_id_pm&, SPU_GRAPH::id_t id) { return id; }
    SpuUltraGraph::edge_descriptor get(edge_index_t tag, const SpuUltraGraph &g, SPU_GRAPH::id_t id) { return id; }

    template <>
    struct property_map<SpuUltraGraph, edge_index_t> {
        typedef spu_ug_edge_id_pm const_type;
        typedef const_type type;
    };


    typedef spu_ug_property_map<vertex_all_t, pair<SpuUltraGraph::vertex_descriptor, value_t>> spu_ug_vertex_data_pm;
    typedef const spu_ug_readable_property_map<vertex_all_t, pair<SpuUltraGraph::vertex_descriptor, value_t>> spu_ug_vertex_data_pm_const;
    spu_ug_vertex_data_pm get(vertex_all_t, SpuUltraGraph &g) { return {&g}; }
    spu_ug_vertex_data_pm_const get(vertex_all_t, const SpuUltraGraph &g) { return {&g}; }
    pair<SpuUltraGraph::vertex_descriptor, value_t> get(spu_ug_vertex_data_pm_const &pm, SPU_GRAPH::id_t id) {
        return {id, pm.get_graph()->get_vertex_value(id)};
    }
    inline pair<SpuUltraGraph::vertex_descriptor, value_t> get(vertex_all_t tag, const SpuUltraGraph &g, SPU_GRAPH::id_t id) { return get(get(tag, g), id); }
    void put(spu_ug_vertex_data_pm pm, SPU_GRAPH::id_t id, pair<SpuUltraGraph::vertex_descriptor, value_t> value) {
        pm.get_graph()->put_vertex(id, value.second);
    }
    inline void put(vertex_all_t tag, SpuUltraGraph &g, SPU_GRAPH::id_t id, pair<SpuUltraGraph::vertex_descriptor, value_t> value) { put(get(tag, g), id, value); }

    template <>
    struct property_map<SpuUltraGraph, vertex_all_t> {
        typedef spu_ug_vertex_data_pm type;
        typedef spu_ug_vertex_data_pm_const const_type;
    };


    typedef spu_ug_property_map<edge_all_t, pair<SpuUltraGraph::edge_descriptor, value_t>> spu_ug_edge_data_pm;
    typedef const spu_ug_readable_property_map<edge_all_t, pair<SpuUltraGraph::edge_descriptor, value_t>> spu_ug_edge_data_pm_const;
    spu_ug_edge_data_pm get(edge_all_t, SpuUltraGraph &g) { return {&g}; }
    spu_ug_edge_data_pm_const get(edge_all_t, const SpuUltraGraph &g) { return {&g}; }
    pair<SpuUltraGraph::edge_descriptor, value_t> get(spu_ug_edge_data_pm_const &pm, SPU_GRAPH::id_t id) {
        return {id, pm.get_graph()->get_edge_value(id)};
    }
    inline pair<SpuUltraGraph::edge_descriptor, value_t> get(edge_all_t tag, const SpuUltraGraph &g, SPU_GRAPH::id_t id) { return get(get(tag, g), id); }
    void put(spu_ug_edge_data_pm pm, SPU_GRAPH::id_t id, pair<SpuUltraGraph::edge_descriptor, value_t> value) {
        pm.get_graph()->put_edge(id, value.second);
    }
    inline void put(edge_all_t tag, SpuUltraGraph &g, SPU_GRAPH::id_t id, pair<SpuUltraGraph::edge_descriptor, value_t> value) { put(get(tag, g), id, value); }

    template <>
    struct property_map<SpuUltraGraph, edge_all_t> {
        typedef spu_ug_edge_data_pm type;
        typedef spu_ug_edge_data_pm_const const_type;
    };


    typedef spu_ug_readable_property_map<edge_weight_t, weight_t> spu_ug_weight_pm;
    spu_ug_weight_pm get(edge_weight_t, const SpuUltraGraph &g) { return {&g}; }
    weight_t get(const spu_ug_weight_pm &pm, SPU_GRAPH::id_t id) { return pm.get_graph()->get_weight(id); }
    inline weight_t get(edge_weight_t tag, const SpuUltraGraph &g, SPU_GRAPH::id_t id) { return get(get(tag, g), id); }

    template <>
    struct property_map<SpuUltraGraph, edge_weight_t> {
        typedef spu_ug_weight_pm const_type;
        typedef const_type type;
    };
}

#endif //GRAPH_API_SPUULTRAGRAPHPROPERTY_H
