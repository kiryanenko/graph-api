//
// Created by kiryanenko on 10.05.19.
//

#include "SpuUltraGraph.h"
#include <math.h>
#include <iostream>


using namespace std;


namespace SPU_GRAPH
{

    SpuUltraGraph::SpuUltraGraph(SPU_GRAPH::id_t graph_id, SPU_GRAPH::SpuUltraGraphTraits spu_graph_traits) :
            _graph_id(graph_id),
            _graph_traits(spu_graph_traits),
            _vertex_fields_len({
                                       {GRAPH_ID, 0},
                                       {INCIDENCE, 1},
                                       {VERTEX_ID, 0},
                                       {WEIGHT, 0},
                                       {EDGE_ID, 0}
                               }),
            _edge_fields_len({
                                     {GRAPH_ID, 0},
                                     {INCIDENCE, 1},
                                     {EDGE_ID, 0},
                                     {VERTEX_ID, 0}
                             })
    {
        if (!_graph_traits.vertex_id_depth || !_graph_traits.edge_id_depth) {
            throw BadRequest("vertex_id_depth and edge_id_depth should be > 0");
        }

        if (_graph_traits.depth_sum() > KEY_DEPTH) {
            throw PayloadTooLarge();
        }

        if (graph_id >= pow(2, _graph_traits.graph_id_depth)) {
            throw PayloadTooLarge();
        }

        _vertex_fields_len[GRAPH_ID] = _graph_traits.graph_id_depth;
        _vertex_fields_len[VERTEX_ID] = _graph_traits.vertex_id_depth;
        _vertex_fields_len[WEIGHT] = _graph_traits.weight_id_depth;
        _vertex_fields_len[EDGE_ID] = _graph_traits.edge_id_depth;

        _edge_fields_len[GRAPH_ID] = _graph_traits.graph_id_depth;
        _edge_fields_len[VERTEX_ID] = _graph_traits.vertex_id_depth;
        _edge_fields_len[EDGE_ID] = _graph_traits.edge_id_depth;

        if (!_graph_traits.vertex_struct) {
            _graph_traits.vertex_struct = new Structure<>;
            _should_free_vertex_struct = true;
        }
        if (!_graph_traits.edge_struct) {
            _graph_traits.edge_struct = new Structure<>;
            _should_free_edge_struct = true;
        }
    }

    SpuUltraGraph::~SpuUltraGraph() {
        if (_should_free_vertex_struct) {
            delete _graph_traits.vertex_struct;
        }
        if (_should_free_edge_struct) {
            delete _graph_traits.edge_struct;
        }
    }

    SpuUltraGraph::vertex_descriptor SpuUltraGraph::add_vertex() {
        auto key = Fields(_vertex_fields_len);
        key[GRAPH_ID] = _graph_id;
        key[VERTEX_ID] = get_free_vertex_id();
        _graph_traits.vertex_struct->insert(key, _graph_traits.default_vertex_value);
    }

    SpuUltraGraph::vertices_size_type SpuUltraGraph::vertices_count() {
        auto key = Fields(_vertex_fields_len);
        key[GRAPH_ID] = _graph_id;
        auto res = _graph_traits.vertex_struct->search(key);
        if (res.status == OK) {
            return res.value;
        } else {
            return 0;
        }
    }

    id_t SpuUltraGraph::get_free_vertex_id() {
        id_t id = vertices_count();
        if (id == 0) ++id;
        if (!is_vertex_id_valid(id)) {
            throw InsufficientStorage();
        }

        auto key = Fields(_vertex_fields_len);
        key[GRAPH_ID] = _graph_id;
        key[VERTEX_ID] = id;
        auto is_free = _graph_traits.vertex_struct->search(key);

        while (is_free.status == OK) {
            ++id;
            if (!is_vertex_id_valid(id)) {
                id = 1;
            }

            key[VERTEX_ID] = id;
            is_free = _graph_traits.vertex_struct->search(key);
        }
        return id;
    }

    bool SpuUltraGraph::is_vertex_id_valid(id_t id) {
        return id > 0 && id < pow(2, _graph_traits.vertex_id_depth) - 1;
    }
}