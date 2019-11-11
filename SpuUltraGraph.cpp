//
// Created by kiryanenko on 10.05.19.
//

#include "SpuUltraGraph.h"
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

        _vertex_fields_len[GRAPH_ID] = _graph_traits.graph_id_depth;
        _vertex_fields_len[VERTEX_ID] = _graph_traits.vertex_id_depth;
        _vertex_fields_len[WEIGHT] = _graph_traits.weight_id_depth;
        _vertex_fields_len[EDGE_ID] = _graph_traits.edge_id_depth;

        _edge_fields_len[GRAPH_ID] = _graph_traits.graph_id_depth;
        _edge_fields_len[VERTEX_ID] = _graph_traits.vertex_id_depth;
        _edge_fields_len[EDGE_ID] = _graph_traits.edge_id_depth;

        if (graph_id >= (id_t) _vertex_fields_len.fieldMask(GRAPH_ID)) {
            throw PayloadTooLarge();
        }

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
        return add_vertex(_graph_traits.default_vertex_value);
    }

    SpuUltraGraph::vertex_descriptor SpuUltraGraph::add_vertex(value_t value) {
        auto id = get_free_vertex_id();
        return add_vertex(id, value);
    }

    SpuUltraGraph::vertex_descriptor SpuUltraGraph::add_vertex(id_t id) {
        return add_vertex(id, _graph_traits.default_vertex_value);
    }

    SpuUltraGraph::vertex_descriptor SpuUltraGraph::add_vertex(id_t id, value_t value) {
        auto key = graph_fields();
        key[VERTEX_ID] = id;
        _graph_traits.vertex_struct->insert(key, value);
        inc_verteces_cnt();
        return id;
    }


    SpuUltraGraph::vertices_size_type SpuUltraGraph::vertices_count() {
        auto key = graph_fields();
        auto res = _graph_traits.vertex_struct->search(key);
        check_spu_resp(res);
        if (res.status == OK) {
            return res.value;
        } else {
            return 0;
        }
    }

    id_t SpuUltraGraph::get_free_vertex_id() {
        if (is_vertex_id_valid(_free_vertex_id)) {
            auto key = graph_fields();
            key[VERTEX_ID] = _free_vertex_id;
            auto is_free = _graph_traits.vertex_struct->search(key);
            if (is_free.status == ERR) {
                return _free_vertex_id++;
            }
        }

        auto id = get_free_vertex_id(1, _vertex_fields_len.fieldMask(VERTEX_ID));
        if (id == 0) {
            throw InsufficientStorage();
        }
        _free_vertex_id = id + 1;
        return id;
    }

    id_t SpuUltraGraph::get_free_vertex_id(id_t min, id_t max) {
        auto key_f = graph_fields();
        key_f[VERTEX_ID] = max;
        auto key = ((data_t) key_f) + 1;

        auto vertex = _graph_traits.vertex_struct->nsm(key);
        check_spu_resp(vertex);
        key_f = vertex.value;
        id_t id = key_f[VERTEX_ID];

        if (vertex.status == ERR || (id_t) key_f[GRAPH_ID] != _graph_id || id < min) {
            return min;
        }
        if (id == max) {
            id = get_free_vertex_id(min, min + (max - min) / 2);
            if (!id) {
                id = get_free_vertex_id(min + (max - min) / 2 + 1, max);
            }
        }
        return id;
    }

    bool SpuUltraGraph::is_vertex_id_valid(id_t id) {
        return id > 0 && id < (id_t) _vertex_fields_len.fieldMask(VERTEX_ID);
    }

    void SpuUltraGraph::check_spu_resp(pair_t resp) {
        if (resp.status == QERR) {
            throw InternalServerError("500 Queue error");
        } else if (resp.status == OERR) {
            throw InternalServerError("500 Command overflow error");
        }
    }

    void SpuUltraGraph::inc_verteces_cnt() {
        auto cnt = vertices_count();
        auto key = graph_fields();
        _graph_traits.vertex_struct->insert(key, cnt + 1);
    }

    void SpuUltraGraph::dec_verteces_cnt() {
        auto cnt = vertices_count();
        auto key = graph_fields();
        _graph_traits.vertex_struct->insert(key, cnt - 1);
    }

    SpuUltraGraph::Fields SpuUltraGraph::graph_fields() {
        auto key = Fields(_vertex_fields_len);
        key[GRAPH_ID] = _graph_id;
        return key;
    }
}