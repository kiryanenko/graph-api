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
            throw BadRequest("Key depth exceed SPU depth");
        }

        _vertex_fields_len[GRAPH_ID] = _graph_traits.graph_id_depth;
        _vertex_fields_len[VERTEX_ID] = _graph_traits.vertex_id_depth;
        _vertex_fields_len[WEIGHT] = _graph_traits.weight_id_depth;
        _vertex_fields_len[EDGE_ID] = _graph_traits.edge_id_depth;

        _edge_fields_len[GRAPH_ID] = _graph_traits.graph_id_depth;
        _edge_fields_len[VERTEX_ID] = _graph_traits.vertex_id_depth;
        _edge_fields_len[EDGE_ID] = _graph_traits.edge_id_depth;

        if (graph_id > (id_t) _vertex_fields_len.fieldMask(GRAPH_ID)) {
            throw BadRequest("graph_id exceed graph_id_depth");
        }

        if (!_graph_traits.vertex_struct) {
            _graph_traits.vertex_struct = new Structure<>;
            _should_free_vertex_struct = true;
        }
        if (!_graph_traits.edge_struct) {
            _graph_traits.edge_struct = new Structure<>;
            _should_free_edge_struct = true;
        }
        
        _vertex_struct.set(_graph_traits.vertex_struct);
        _edge_struct.set(_graph_traits.edge_struct);
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
        return add_vertex(id, value, true);
    }

    SpuUltraGraph::vertex_descriptor SpuUltraGraph::add_vertex(id_t id, bool safe) {
        return add_vertex(id, _graph_traits.default_vertex_value, safe);
    }

    SpuUltraGraph::vertex_descriptor SpuUltraGraph::add_vertex(id_t id, value_t value, bool safe) {
        if (!is_vertex_id_valid(id)) {
            throw BadRequest("Invalid ID");
        }

        auto key = vertex_key(id);

        if (!safe) {
            auto conflict = _vertex_struct.search(key);
            if (conflict.status == OK) {
                throw Conflict();
            }
        }

        _vertex_struct.insert(key, value);

        key[WEIGHT] = _vertex_fields_len.fieldMask(WEIGHT);
        key[EDGE_ID] = _vertex_fields_len.fieldMask(EDGE_ID);
        _vertex_struct.insert(key, 0);

        key[INCIDENCE] = 1;
        _vertex_struct.insert(key, 0);

        inc_verteces_cnt();
        return id;
    }


    SpuUltraGraph::vertices_size_type SpuUltraGraph::num_vertices() {
        auto key = vertex_key();
        auto res = _vertex_struct.search(key);
        if (res.status == OK) {
            return res.value;
        } else {
            return 0;
        }
    }

    id_t SpuUltraGraph::get_free_vertex_id() {
        if (is_vertex_id_valid(_free_vertex_id)) {
            auto key = vertex_key();
            key[VERTEX_ID] = _free_vertex_id;
            auto is_free = _vertex_struct.search(key);
            if (is_free.status == ERR) {
                return _free_vertex_id++;
            }
        }

        auto id = get_free_vertex_id(1, _vertex_fields_len.fieldMask(VERTEX_ID) - 1);
        if (id == 0) {
            throw InsufficientStorage();
        }
        _free_vertex_id = id + 1;
        return id;
    }

    id_t SpuUltraGraph::get_free_vertex_id(id_t min, id_t max) const {
        auto key_f = vertex_key();
        key_f[VERTEX_ID] = max;
        auto key = (data_t) key_f + 1;

        auto vertex = _vertex_struct.nsm(key);
        key_f = vertex.key;
        id_t id = key_f[VERTEX_ID];

        if (vertex.status == ERR || (id_t) key_f[GRAPH_ID] != _graph_id || id < min) {
            return min;
        }

        if (id != max) {
            return id + 1;
        }

        id = get_free_edge_id(min, min + (max - min) / 2);
        if (!id) {
            id = get_free_edge_id(min + (max - min) / 2 + 1, max);
        }
        return id;
    }

    bool SpuUltraGraph::is_vertex_id_valid(id_t id) const {
        return id > 0 && id < (id_t) _vertex_fields_len.fieldMask(VERTEX_ID);
    }

    void SpuUltraGraph::inc_verteces_cnt() {
        auto cnt = num_vertices();
        auto key = vertex_key();
        _vertex_struct.insert(key, cnt + 1);
    }

    void SpuUltraGraph::dec_verteces_cnt() {
        auto cnt = num_vertices();
        auto key = vertex_key();
        _vertex_struct.insert(key, cnt - 1);
    }


    SpuUltraGraph::Fields SpuUltraGraph::vertex_key(id_t vertex, uint8_t incidence, weight_t weight, id_t edge) const {
        auto key = Fields(_vertex_fields_len);
        key[GRAPH_ID] = _graph_id;
        key[INCIDENCE] = incidence;
        key[VERTEX_ID] = vertex;
        key[WEIGHT] = weight;
        key[EDGE_ID] = edge;
        return key;
    }

    SpuUltraGraph::Fields SpuUltraGraph::edge_key(id_t edge, uint8_t incidence, id_t vertex) const {
        auto key = Fields(_edge_fields_len);
        key[GRAPH_ID] = _graph_id;
        key[INCIDENCE] = incidence;
        key[EDGE_ID] = edge;
        key[VERTEX_ID] = vertex;
        return key;
    }


    SpuUltraGraph::edge_descriptor SpuUltraGraph::add_edge() {
        return add_weight_edge(_graph_traits.default_weight);
    }

    SpuUltraGraph::edge_descriptor SpuUltraGraph::add_weight_edge(weight_t weight) {
        auto id = get_free_edge_id();
        return add_weight_edge(id, weight, true);
    }

    SpuUltraGraph::edge_descriptor SpuUltraGraph::add_edge(id_t id, bool safe) {
        return add_weight_edge(id, _graph_traits.default_weight, safe);
    }


    SpuUltraGraph::edge_descriptor SpuUltraGraph::add_weight_edge(id_t id, weight_t weight, bool safe) {
        if (!is_edge_id_valid(id)) {
            throw BadRequest("Invalid ID");
        }

        auto key = edge_key(id);

        if (!safe) {
            auto conflict = _edge_struct.search(key);
            if (conflict.status == OK) {
                throw Conflict();
            }
        }

        _edge_struct.insert(key, weight);
        _edge_struct.insert(source_cnt_key(id), 0);
        _edge_struct.insert(target_cnt_key(id), 0);

        inc_edges_cnt();
        return id;
    }

    SpuUltraGraph::edge_descriptor
    SpuUltraGraph::add_edge(SpuUltraGraph::vertex_descriptor from, SpuUltraGraph::vertex_descriptor to, bool safe) {
        return add_weight_edge(from, to, _graph_traits.default_weight, safe);

    }

    SpuUltraGraph::edge_descriptor
    SpuUltraGraph::add_weight_edge(SpuUltraGraph::vertex_descriptor from, SpuUltraGraph::vertex_descriptor to,
                                   weight_t weight, bool safe) {
        auto id = get_free_edge_id();
        if (!safe && (!has_vertex(from) || !has_vertex(to))) {
            throw NotFound();
        }
        return add_weight_edge(id, from, to, weight, true);
    }

    SpuUltraGraph::edge_descriptor
    SpuUltraGraph::add_edge(id_t id, SpuUltraGraph::vertex_descriptor from, SpuUltraGraph::vertex_descriptor to, bool safe) {
        return add_weight_edge(id, from, to, _graph_traits.default_weight, safe);
    }

    SpuUltraGraph::edge_descriptor
    SpuUltraGraph::add_weight_edge(id_t id, SpuUltraGraph::vertex_descriptor from, SpuUltraGraph::vertex_descriptor to,
                                   weight_t weight, bool safe) {
        add_weight_edge(id, weight, safe);

        if (!safe && (!has_vertex(from) || !has_vertex(to))) {
            throw NotFound();
        }
        add_source(id, from, weight);
        add_target(id, to, weight);
        return id;
    }


    SpuUltraGraph::edges_size_type SpuUltraGraph::num_edges() {
        auto key = edge_key();
        auto res = _edge_struct.search(key);
        if (res.status == OK) {
            return res.value;
        } else {
            return 0;
        }
    }

    bool SpuUltraGraph::is_edge_id_valid(id_t id) const {
        return id > 0 && id < (id_t) _vertex_fields_len.fieldMask(EDGE_ID);
    }


    id_t SpuUltraGraph::get_free_edge_id() {
        if (is_edge_id_valid(_free_edge_id)) {
            auto key = edge_key();
            key[EDGE_ID] = _free_edge_id;
            auto is_free = _edge_struct.search(key);
            if (is_free.status == ERR) {
                return _free_edge_id++;
            }
        }

        auto id = get_free_edge_id(1, _edge_fields_len.fieldMask(EDGE_ID) - 1);
        if (id == 0) {
            throw InsufficientStorage();
        }
        _free_edge_id = id + 1;
        return id;
    }

    id_t SpuUltraGraph::get_free_edge_id(id_t min, id_t max) const {
        auto key_f = edge_key();
        key_f[EDGE_ID] = max;
        auto key = (data_t) key_f + 1;

        auto edge = _edge_struct.nsm(key);
        key_f = edge.key;
        id_t id = key_f[EDGE_ID];

        if (edge.status == ERR || (id_t) key_f[GRAPH_ID] != _graph_id || id < min) {
            return min;
        }

        if (id != max) {
            return id + 1;
        }

        id = get_free_edge_id(min, min + (max - min) / 2);
        if (!id) {
            id = get_free_edge_id(min + (max - min) / 2 + 1, max);
        }
        return id;
    }


    void SpuUltraGraph::inc_edges_cnt() {
        auto cnt = num_edges();
        auto key = edge_key();
        _edge_struct.insert(key, cnt + 1);
    }

    void SpuUltraGraph::dec_edges_cnt() {
        auto cnt = num_edges();
        auto key = edge_key();
        _edge_struct.insert(key, cnt - 1);
    }

    bool SpuUltraGraph::has_vertex(id_t id) {
        auto key = vertex_key();
        key[VERTEX_ID] = id;
        auto res = _vertex_struct.search(key);
        return res.status == OK;
    }

    bool SpuUltraGraph::has_edge(id_t id) {
        auto key = edge_key();
        key[EDGE_ID] = id;
        auto res = _edge_struct.search(key);
        return res.status == OK;
    }

    void
    SpuUltraGraph::add_target(SpuUltraGraph::edge_descriptor edge, SpuUltraGraph::vertex_descriptor vertex, bool safe) {
        if (!safe && !has_vertex(vertex)) {
            throw NotFound();
        }
        add_target(edge, vertex, get_weight(edge));
    }

    void
    SpuUltraGraph::add_source(SpuUltraGraph::edge_descriptor edge, SpuUltraGraph::vertex_descriptor vertex, bool safe) {
        if (!safe && !has_vertex(vertex)) {
            throw NotFound();
        }
        add_source(edge, vertex, get_weight(edge));
    }

    void SpuUltraGraph::add_target(SpuUltraGraph::edge_descriptor edge, SpuUltraGraph::vertex_descriptor vertex,
                                   weight_t weight) {
        auto v_key = vertex_key();
        v_key[INCIDENCE] = 1;
        v_key[VERTEX_ID] = vertex;
        v_key[WEIGHT] = weight;
        v_key[EDGE_ID] = edge;
        _vertex_struct.insert(v_key, 0);

        auto e_key = edge_key();
        e_key[INCIDENCE] = 1;
        e_key[EDGE_ID] = edge;
        e_key[VERTEX_ID] = vertex;
        _edge_struct.insert(e_key, 0);

        inc_in_degree(vertex);
        inc_target_cnt(edge);
    }

    void SpuUltraGraph::add_source(SpuUltraGraph::edge_descriptor edge, SpuUltraGraph::vertex_descriptor vertex,
                                   weight_t weight) {
        auto v_key = vertex_key();
        v_key[VERTEX_ID] = vertex;
        v_key[WEIGHT] = weight;
        v_key[EDGE_ID] = edge;
        _vertex_struct.insert(v_key, 0);

        auto e_key = edge_key();
        e_key[EDGE_ID] = edge;
        e_key[VERTEX_ID] = vertex;
        _edge_struct.insert(e_key, 0);

        inc_out_degree(vertex);
        inc_source_cnt(edge);
    }

    weight_t SpuUltraGraph::get_weight(SpuUltraGraph::edge_descriptor edge) {
        if (_vertex_fields_len[WEIGHT] == 0) {
            return 0;
        }

        auto key = edge_key();
        key[EDGE_ID] = edge;
        auto res = _edge_struct.search(key);
        if (res.status == ERR) {
            throw NotFound();
        }
        return res.value;
    }

    void SpuUltraGraph::remove_edge(SpuUltraGraph::edge_descriptor edge) {
        try {
            auto v_key = vertex_key();
            v_key[WEIGHT] = get_weight(edge);
            v_key[EDGE_ID] = edge;

            auto e_key = edge_key();
            auto start = edge_key(edge, 0, 1);
            auto end = edge_key(edge, 0, max_vertex_id());
            for (auto pair : StructureRange(&_edge_struct, start, end)) {
                e_key = pair.key;
                vertex_descriptor v = e_key[VERTEX_ID];
                v_key[VERTEX_ID] =  v;
                _vertex_struct.del(v_key);
                _edge_struct.del(e_key);
                dec_out_degree(v);
            }
            v_key[INCIDENCE] = 1;
            start[INCIDENCE] = 1;
            end[INCIDENCE] = 1;
            for (auto pair : StructureRange(&_edge_struct, start, end)) {
                e_key = pair.key;
                vertex_descriptor v = e_key[VERTEX_ID];
                v_key[VERTEX_ID] =  v;
                _vertex_struct.del(v_key);
                _edge_struct.del(e_key);
                dec_in_degree(v);
            }

            _edge_struct.del(edge_key(edge));
            dec_edges_cnt();
            _edge_struct.del(target_cnt_key(edge));
            _edge_struct.del(source_cnt_key(edge));
        } catch (exception &) {}
    }

    /// Удаляются все соединения от вершины from к to.
    /// Если ребро не содержит вершин, то оно полностью удаляется
    void SpuUltraGraph::remove_edge(SpuUltraGraph::vertex_descriptor from, SpuUltraGraph::vertex_descriptor to) {
        auto u_vertex_key = vertex_key(from);
        auto v_vertex_key = vertex_key(to, 1);
        auto u_edge_key = edge_key(0, 0, to);
        auto v_edge_key = edge_key(0, 1, from);
        for (auto edge : parallel_edges(from, to)) {
            u_vertex_key[EDGE_ID] = edge.first;
            u_vertex_key[WEIGHT] = edge.second;
            _vertex_struct.del(u_vertex_key);
            v_vertex_key[EDGE_ID] = edge.first;
            v_vertex_key[WEIGHT] = edge.second;
            _vertex_struct.del(v_vertex_key);

            u_edge_key[EDGE_ID] = edge.first;
            _edge_struct.del(u_edge_key);
            v_edge_key[EDGE_ID] = edge.first;
            _edge_struct.del(v_edge_key);
        }
    }

    SpuUltraGraph::ParallelEdges
    SpuUltraGraph::parallel_edges(SpuUltraGraph::vertex_descriptor from, SpuUltraGraph::vertex_descriptor to) const {
        return {this, from, to};
    }

    void SpuUltraGraph::clear_vertex(SpuUltraGraph::vertex_descriptor v) {

    }


    void SpuUltraGraph::inc_out_degree(SpuUltraGraph::vertex_descriptor v, size_t val) {
        auto key = out_degree_key(v);
        auto cnt = _vertex_struct.search(key);
        if (cnt.status == ERR) {
            throw NotFound();
        }
        _vertex_struct.insert(key, cnt.value + val);
    }

    void SpuUltraGraph::dec_out_degree(SpuUltraGraph::vertex_descriptor v, size_t val) {
        auto key = out_degree_key(v);
        auto cnt = _vertex_struct.search(key);
        if (cnt.status == ERR) {
            throw NotFound();
        }
        _vertex_struct.insert(key, cnt.value - val);
    }

    void SpuUltraGraph::inc_in_degree(SpuUltraGraph::vertex_descriptor v, size_t val) {
        auto key = in_degree_key(v);
        auto cnt = _vertex_struct.search(key);
        if (cnt.status == ERR) {
            throw NotFound();
        }
        _vertex_struct.insert(key, cnt.value + val);
    }

    void SpuUltraGraph::dec_in_degree(SpuUltraGraph::vertex_descriptor v, size_t val) {
        auto key = in_degree_key(v);
        auto cnt = _vertex_struct.search(key);
        if (cnt.status == ERR) {
            throw NotFound();
        }
        _vertex_struct.insert(key, cnt.value - val);
    }


    void SpuUltraGraph::inc_target_cnt(SpuUltraGraph::edge_descriptor e, size_t val) {
        auto key = target_cnt_key(e);
        auto cnt = _edge_struct.search(key);
        if (cnt.status == ERR) {
            throw NotFound();
        }
        _vertex_struct.insert(key, cnt.value + val);
    }

    void SpuUltraGraph::dec_target_cnt(SpuUltraGraph::edge_descriptor e, size_t val) {
        auto key = target_cnt_key(e);
        auto cnt = _edge_struct.search(key);
        if (cnt.status == ERR) {
            throw NotFound();
        }
        _vertex_struct.insert(key, cnt.value - val);
    }

    void SpuUltraGraph::inc_source_cnt(SpuUltraGraph::edge_descriptor e, size_t val) {
        auto key = source_cnt_key(e);
        auto cnt = _edge_struct.search(key);
        if (cnt.status == ERR) {
            throw NotFound();
        }
        _vertex_struct.insert(key, cnt.value + val);
    }

    void SpuUltraGraph::dec_source_cnt(SpuUltraGraph::edge_descriptor e, size_t val) {
        auto key = source_cnt_key(e);
        auto cnt = _edge_struct.search(key);
        if (cnt.status == ERR) {
            throw NotFound();
        }
        _vertex_struct.insert(key, cnt.value - val);
    }


    SpuUltraGraph::Fields SpuUltraGraph::out_degree_key(SpuUltraGraph::vertex_descriptor v) const {
        return vertex_key(v, 0, _vertex_fields_len.fieldMask(WEIGHT), _vertex_fields_len.fieldMask(EDGE_ID));
    }

    SpuUltraGraph::Fields SpuUltraGraph::in_degree_key(SpuUltraGraph::vertex_descriptor v) const {
        return vertex_key(v, 1, _vertex_fields_len.fieldMask(WEIGHT), _vertex_fields_len.fieldMask(EDGE_ID));
    }

    SpuUltraGraph::Fields SpuUltraGraph::target_cnt_key(SpuUltraGraph::edge_descriptor e) const {
        return edge_key(e, 1, _edge_fields_len.fieldMask(VERTEX_ID));
    }

    SpuUltraGraph::Fields SpuUltraGraph::source_cnt_key(SpuUltraGraph::edge_descriptor e) const {
        return edge_key(e, 0, _edge_fields_len.fieldMask(VERTEX_ID));
    }

    id_t SpuUltraGraph::max_vertex_id() const {
        return _vertex_fields_len.fieldMask(VERTEX_ID) - 1;
    }

    id_t SpuUltraGraph::max_edge_id() const {
        return _edge_fields_len.fieldMask(EDGE_ID) - 1;
    }


    void SpuUltraGraph::ParallelEdgesIterator::increment() {
        auto u_fields = _graph->vertex_key(_from, 0, _weight, _edge);
        auto v_fields = _graph->vertex_key();
        auto resp = _graph->_vertex_struct.ngr(u_fields);
        u_fields = resp.key;
        while (u_fields[EDGE_ID] != v_fields[EDGE_ID]) {
            if (resp.status == ERR
                || (id_t) u_fields[GRAPH_ID] != _graph->_graph_id
                || (id_t) u_fields[VERTEX_ID] != _to
                || !_graph->is_edge_id_valid(u_fields[EDGE_ID])) {
                _edge = 0;
                _weight = 0;
                return;
            }

            if (u_fields[WEIGHT] > v_fields[WEIGHT]
                || (u_fields[WEIGHT] == v_fields[WEIGHT] && u_fields[EDGE_ID] > v_fields[EDGE_ID])) {
                v_fields = u_fields;
                v_fields[VERTEX_ID] = _to;
                v_fields[INCIDENCE] = 1;
                resp = _graph->_vertex_struct.ngr(u_fields);
                v_fields = resp.key;

            } else {
                u_fields = v_fields;
                u_fields[VERTEX_ID] = _from;
                u_fields[INCIDENCE] = 0;
                resp = _graph->_vertex_struct.ngr(v_fields);
                u_fields = resp.key;
            }
        }

        _edge = u_fields[EDGE_ID];
        _weight = u_fields[WEIGHT];
    }

    void SpuUltraGraph::ParallelEdgesIterator::decrement() {
        auto u_fields = _graph->vertex_key(_from, 0, _weight, _edge);
        auto v_fields = _graph->vertex_key();
        auto resp = _graph->_vertex_struct.nsm(u_fields);
        u_fields = resp.key;
        while (u_fields[EDGE_ID] != v_fields[EDGE_ID]) {
            if (resp.status == ERR
                || (id_t) u_fields[GRAPH_ID] != _graph->_graph_id
                || (id_t) u_fields[VERTEX_ID] != _to
                || !_graph->is_edge_id_valid(u_fields[EDGE_ID])) {
                _edge = 0;
                _weight = 0;
                return;
            }

            if (u_fields[WEIGHT] < v_fields[WEIGHT]
                || (u_fields[WEIGHT] == v_fields[WEIGHT] && u_fields[EDGE_ID] < v_fields[EDGE_ID])) {
                v_fields = u_fields;
                v_fields[VERTEX_ID] = _to;
                v_fields[INCIDENCE] = 1;
                resp = _graph->_vertex_struct.nsm(u_fields);
                v_fields = resp.key;
            } else {
                u_fields = v_fields;
                u_fields[VERTEX_ID] = _from;
                u_fields[INCIDENCE] = 0;
                resp = _graph->_vertex_struct.nsm(v_fields);
                u_fields = resp.key;
            }
        }

        _edge = u_fields[EDGE_ID];
        _weight = u_fields[WEIGHT];
    }

    pair<SpuUltraGraph::edge_descriptor, weight_t> SpuUltraGraph::ParallelEdgesIterator::dereference() const {
        return {_edge, _weight};
    }

    void SpuUltraGraph::AdjacentEdgesIterator::increment() {
        auto key = _g->vertex_key(_v, _incidence, _weight, _edge);
        auto resp = _g->_vertex_struct.ngr(key);
        if (resp.status == ERR) {
            _v = 0;
            _incidence = 0;
            _weight = 0;
            _edge = 0;
            return;
        }

        key = resp.key;
        _v = key[VERTEX_ID];
        _incidence = key[INCIDENCE];
        _weight = key[WEIGHT];
        _edge = key[EDGE_ID];
    }

    void SpuUltraGraph::AdjacentEdgesIterator::decrement() {
        auto key = _g->vertex_key(_v, _incidence, _weight, _edge);
        auto resp = _g->_vertex_struct.nsm(key);
        if (resp.status == ERR) {
            _v = 0;
            _incidence = 0;
            _weight = 0;
            _edge = 0;
            return;
        }

        key = resp.key;
        _v = key[VERTEX_ID];
        _incidence = key[INCIDENCE];
        _weight = key[WEIGHT];
        _edge = key[EDGE_ID];
    }

    bool SpuUltraGraph::AdjacentEdgesIterator::equal(const SpuUltraGraph::AdjacentEdgesIterator &other) const {
        return _edge == other._edge && _v == other._v && _weight == other._weight;
    }
}