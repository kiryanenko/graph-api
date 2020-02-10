//
// Created by kiryanenko on 10.05.19.
//

#include "SpuUltraGraph.h"
#include <iostream>
#include <cstdio>


using namespace std;


namespace SPU_GRAPH
{

    SpuUltraGraph::SpuUltraGraph(SPU_GRAPH::id_t graph_id, SPU_GRAPH::SpuUltraGraphTraits spu_graph_traits) :
            _graph_id(graph_id),
            _graph_traits(spu_graph_traits),
            _edge_id_fields_len({
                                       {EDGE_ID, 0},
                                       {WEIGHT, 0},
                               }),
            _vertex_fields_len({
                                       {EDGE_ID, 0},
                                       {VERTEX_ID, 0},
                                       {INCIDENCE, 1},
                                       {GRAPH_ID, 0},
                               }),
            _edge_fields_len({
                                     {VERTEX_ID, 0},
                                     {EDGE_ID, 0},
                                     {INCIDENCE, 1},
                                     {GRAPH_ID, 0}
                             })
    {
        if (!_graph_traits.vertex_id_depth || !_graph_traits.edge_id_depth) {
            throw BadRequest("vertex_id_depth and edge_id_depth should be > 0");
        }

        if (_graph_traits.depth_sum() > KEY_DEPTH) {
            throw BadRequest("Key depth exceed SPU depth");
        }

        auto edge_id_depth = _graph_traits.edge_id_depth + _graph_traits.weight_depth;
        _edge_id_fields_len[EDGE_ID] = _graph_traits.edge_id_depth;
        _edge_id_fields_len[WEIGHT] = _graph_traits.weight_depth;

        _vertex_fields_len[GRAPH_ID] = _graph_traits.graph_id_depth;
        _vertex_fields_len[VERTEX_ID] = _graph_traits.vertex_id_depth;
        _vertex_fields_len[EDGE_ID] = edge_id_depth;

        _edge_fields_len[GRAPH_ID] = _graph_traits.graph_id_depth;
        _edge_fields_len[VERTEX_ID] = _graph_traits.vertex_id_depth;
        _edge_fields_len[EDGE_ID] = edge_id_depth;

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

        key[EDGE_ID] = _vertex_fields_len.fieldMask(EDGE_ID);
        _vertex_struct.insert(key, 0);

        key[INCIDENCE] = 1;
        _vertex_struct.insert(key, 0);

        inc_vertexes_cnt();
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

        auto id = get_free_vertex_id(1, max_vertex_id());
        if (id == 0) {
            throw InsufficientStorage();
        }
        _free_vertex_id = id + 1;
        return id;
    }

    id_t SpuUltraGraph::get_free_vertex_id(id_t min, id_t max) const {
        auto key = vertex_key(max, 0, 1);
        auto vertex = _vertex_struct.nsm(key);
        key = vertex.key;
        id_t id = key[VERTEX_ID];

        if (vertex.status == ERR || (id_t) key[GRAPH_ID] != _graph_id || id < min) {
            return min;
        }

        if (id != max) {
            return id + 1;
        }

        id = get_free_vertex_id(min, min + (max - min) / 2);
        if (!id) {
            id = get_free_vertex_id(min + (max - min) / 2 + 1, max);
        }
        return id;
    }

    bool SpuUltraGraph::is_vertex_id_valid(id_t id) const {
        return id > 0 && id <= max_vertex_id();
    }

    SpuUltraGraph::vertices_size_type SpuUltraGraph::inc_vertexes_cnt() {
        auto cnt = num_vertices() + 1;
        auto key = vertex_key();
        _vertex_struct.insert(key, cnt);
        return cnt;
    }

    SpuUltraGraph::vertices_size_type SpuUltraGraph::dec_vertexes_cnt() {
        auto cnt = num_vertices() - 1;
        auto key = vertex_key();
        _vertex_struct.insert(key, cnt);
        return cnt;
    }


    SpuUltraGraph::Fields SpuUltraGraph::vertex_key(id_t vertex, uint8_t incidence, id_t edge) const {
        auto key = Fields(_vertex_fields_len);
        key[GRAPH_ID] = _graph_id;
        key[INCIDENCE] = incidence;
        key[VERTEX_ID] = vertex;
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
        auto id = get_free_edge_id();
        return add_edge(id, true);
    }

    SpuUltraGraph::edge_descriptor SpuUltraGraph::add_edge(id_t id, bool safe) {
        if (!is_edge_id_valid(id)) {
            throw BadRequest("Invalid ID");
        }
        if (!safe && has_edge(id)) {
            throw Conflict();
        }

        _edge_struct.insert(source_cnt_key(id), 0);
        _edge_struct.insert(target_cnt_key(id), 0);

        inc_edges_cnt();
        return id;
    }

    SpuUltraGraph::edge_descriptor
    SpuUltraGraph::add_edge(SpuUltraGraph::vertex_descriptor from, SpuUltraGraph::vertex_descriptor to, bool safe) {
        auto id = get_free_edge_id();
        if (!safe && (!has_vertex(from) || !has_vertex(to))) {
            throw NotFound();
        }
        return add_edge(id, from, to, true);
    }

    SpuUltraGraph::edge_descriptor
    SpuUltraGraph::add_edge(id_t id, SpuUltraGraph::vertex_descriptor from, SpuUltraGraph::vertex_descriptor to, bool safe) {
        add_edge(id, safe);
        add_source(id, from, safe);
        add_target(id, to, safe);
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
        return id > 0 && id <= max_edge_id();
    }


    id_t SpuUltraGraph::get_free_edge_id() {
        if (is_edge_id_valid(_free_edge_id)) {
            auto key = source_cnt_key(_free_edge_id);
            auto is_free = _edge_struct.search(key);
            if (is_free.status == ERR) {
                return _free_edge_id++;
            }
        }

        auto id = get_free_edge_id(1, max_edge_id());
        if (id == 0) {
            throw InsufficientStorage();
        }
        _free_edge_id = id + 1;
        return id;
    }

    id_t SpuUltraGraph::get_free_edge_id(id_t min, id_t max) const {
        auto key_f = source_cnt_key(max);
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

    bool SpuUltraGraph::has_edge(edge_descriptor id) {
        auto key = source_cnt_key(id);
        auto res = _edge_struct.search(key);
        return res.status == OK;
    }

    void
    SpuUltraGraph::add_target(SpuUltraGraph::edge_descriptor edge, SpuUltraGraph::vertex_descriptor vertex, bool safe) {
        if (!safe && !has_vertex(vertex)) {
            throw NotFound();
        }

        auto v_key = vertex_key(vertex, 1, edge);
        _vertex_struct.insert(v_key);
        auto e_key = edge_key(edge, 1, vertex);
        _edge_struct.insert(e_key);

        inc_in_degree(vertex);
        inc_target_cnt(edge);
    }

    void
    SpuUltraGraph::add_source(SpuUltraGraph::edge_descriptor edge, SpuUltraGraph::vertex_descriptor vertex, bool safe) {
        if (!safe && !has_vertex(vertex)) {
            throw NotFound();
        }

        auto v_key = vertex_key(vertex, 0, edge);
        _vertex_struct.insert(v_key);
        auto e_key = edge_key(edge, 0, vertex);
        _edge_struct.insert(e_key);

        inc_out_degree(vertex);
        inc_source_cnt(edge);
    }

    weight_t SpuUltraGraph::get_weight(SpuUltraGraph::edge_descriptor edge) {
        auto fields = Fields(_edge_id_fields_len);
        return fields[WEIGHT];
    }

    void SpuUltraGraph::remove_edge(SpuUltraGraph::edge_descriptor edge) {
        if (!has_edge(edge)) return;
        auto v_key = vertex_key(0, 0, edge);
        auto e_key = edge_key();
        auto start = edge_key(edge, 0, 1);
        auto end = edge_key(edge, 0, max_vertex_id());
        for (auto pair : StructureRange(&_edge_struct, start, end)) {
            e_key = pair.key;
            vertex_descriptor v = e_key[VERTEX_ID];
            v_key[VERTEX_ID] =  v;
            _vertex_struct.del(v_key);
            _edge_struct.del(e_key);
            try {
                dec_out_degree(v);
            } catch (NotFound &) {}
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
            try {
                dec_in_degree(v);
            } catch (NotFound &) {}
        }

        _edge_struct.del(edge_key(edge));
        dec_edges_cnt();
        _edge_struct.del(target_cnt_key(edge));
        _edge_struct.del(source_cnt_key(edge));
    }

    /// Удаляются все соединения от вершины from к to.
    /// Само ребро НЕ удаляется
    void SpuUltraGraph::remove_edge(SpuUltraGraph::vertex_descriptor from, SpuUltraGraph::vertex_descriptor to) {
        auto from_vertex_key = vertex_key(from);
        auto to_vertex_key = vertex_key(to, 1);
        auto from_edge_key = edge_key(0, 0, from);
        auto to_edge_key = edge_key(0, 1, to);
        size_t removed_edges_cnt = 0;
        for (auto edge_id : parallel_edges(from, to)) {
            from_vertex_key[EDGE_ID] = edge_id;
            _vertex_struct.del(from_vertex_key);
            to_vertex_key[EDGE_ID] = edge_id;
            _vertex_struct.del(to_vertex_key);
            removed_edges_cnt++;

            from_edge_key[EDGE_ID] = edge_id;
            _edge_struct.del(from_edge_key);
            try {
                dec_source_cnt(edge_id);
            } catch (NotFound &) {}

            to_edge_key[EDGE_ID] = edge_id;
            _edge_struct.del(to_edge_key);
            try {
                dec_target_cnt(edge_id);
            } catch (NotFound &) {}
        }
        try {
            dec_out_degree(from, removed_edges_cnt);
        } catch (NotFound &) {}
        try {
            dec_in_degree(to, removed_edges_cnt);
        } catch (NotFound &) {}
    }

    SpuUltraGraph::ParallelEdges
    SpuUltraGraph::parallel_edges(SpuUltraGraph::vertex_descriptor from, SpuUltraGraph::vertex_descriptor to) const {
        return {this, from, to};
    }

    void SpuUltraGraph::clear_vertex(SpuUltraGraph::vertex_descriptor v) {

    }


    SpuUltraGraph::vertices_size_type SpuUltraGraph::inc_out_degree(SpuUltraGraph::vertex_descriptor v, SpuUltraGraph::vertices_size_type val) {
        auto cnt = out_degree(v) + val;
        _vertex_struct.insert(out_degree_key(v), cnt);
        return cnt;
    }

    SpuUltraGraph::vertices_size_type SpuUltraGraph::dec_out_degree(SpuUltraGraph::vertex_descriptor v, SpuUltraGraph::vertices_size_type val) {
        auto cnt = out_degree(v) - val;
        _vertex_struct.insert(out_degree_key(v), cnt);
        return cnt;
    }

    SpuUltraGraph::vertices_size_type SpuUltraGraph::inc_in_degree(SpuUltraGraph::vertex_descriptor v, SpuUltraGraph::vertices_size_type val) {
        auto cnt = in_degree(v) + val;
        _vertex_struct.insert(in_degree_key(v), cnt);
        return cnt;
    }

    SpuUltraGraph::vertices_size_type SpuUltraGraph::dec_in_degree(SpuUltraGraph::vertex_descriptor v, SpuUltraGraph::vertices_size_type val) {
        auto cnt = in_degree(v) - val;
        _vertex_struct.insert(in_degree_key(v), cnt);
        return cnt;
    }


    SpuUltraGraph::edges_size_type SpuUltraGraph::inc_target_cnt(SpuUltraGraph::edge_descriptor e, SpuUltraGraph::edges_size_type val) {
        auto cnt = target_cnt(e) + val;
        _vertex_struct.insert(target_cnt_key(e), cnt);
        return cnt;
    }

    SpuUltraGraph::edges_size_type SpuUltraGraph::dec_target_cnt(SpuUltraGraph::edge_descriptor e, SpuUltraGraph::edges_size_type val) {
        auto cnt = target_cnt(e) - val;
        _vertex_struct.insert(target_cnt_key(e), cnt);
        return cnt;
    }

    SpuUltraGraph::edges_size_type SpuUltraGraph::inc_source_cnt(SpuUltraGraph::edge_descriptor e, SpuUltraGraph::edges_size_type val) {
        auto cnt = source_cnt(e) + val;
        _vertex_struct.insert(source_cnt_key(e), cnt);
        return cnt;
    }

    SpuUltraGraph::edges_size_type SpuUltraGraph::dec_source_cnt(SpuUltraGraph::edge_descriptor e, SpuUltraGraph::edges_size_type val) {
        auto cnt = source_cnt(e) - val;
        _vertex_struct.insert(source_cnt_key(e), cnt);
        return cnt;
    }


    SpuUltraGraph::Fields SpuUltraGraph::out_degree_key(SpuUltraGraph::vertex_descriptor v) const {
        return vertex_key(v, 0, _vertex_fields_len.fieldMask(EDGE_ID));
    }

    SpuUltraGraph::Fields SpuUltraGraph::in_degree_key(SpuUltraGraph::vertex_descriptor v) const {
        return vertex_key(v, 1, _vertex_fields_len.fieldMask(EDGE_ID));
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
        auto d = _edge_fields_len.fieldMask(EDGE_ID);
        return _edge_fields_len.fieldMask(EDGE_ID) - 1;
    }

    id_t SpuUltraGraph::max_weight() const {
        return _edge_id_fields_len.fieldMask(WEIGHT);
    }


    void SpuUltraGraph::print_vertex_struct() const {
        cout << "Vertex Structure" << endl;
        cout << "INCIDENCE  VERTEX_ID    EDGE_ID |      VALUE" << endl;
        auto last_key = vertex_key(_vertex_fields_len.fieldMask(VERTEX_ID), 1, _vertex_fields_len.fieldMask(EDGE_ID));
        auto key = vertex_key();
        for (auto pair : StructureRange(&_vertex_struct, 0, last_key)) {
            key = pair.key;
            printf("%9d %10d %10d | %10d\n", (int) key[INCIDENCE], (int) key[VERTEX_ID], (int) key[EDGE_ID], (int) pair.value);
        }
        cout << endl;
    }

    void SpuUltraGraph::print_edge_struct() const {
        cout << "Edge Structure:" << endl;
        cout << "INCIDENCE    EDGE_ID  VERTEX_ID |      VALUE" << endl;
        auto last_key = edge_key(_edge_fields_len.fieldMask(EDGE_ID), 1,
                _edge_fields_len.fieldMask(VERTEX_ID));
        auto key = edge_key();
        for (auto pair : StructureRange(&_edge_struct, 0, last_key)) {
            key = pair.key;
            printf("%9d %10d %10d | %10d\n", (int) key[INCIDENCE], (int) key[EDGE_ID], (int) key[VERTEX_ID], (int) pair.value);
        }
        cout << endl;
    }

    SpuUltraGraph::vertices_size_type SpuUltraGraph::out_degree(SpuUltraGraph::vertex_descriptor v) {
        auto key = out_degree_key(v);
        auto res = _vertex_struct.search(key);
        if (res.status == ERR) {
            throw NotFound();
        }
        return res.value;
    }

    SpuUltraGraph::vertices_size_type SpuUltraGraph::in_degree(SpuUltraGraph::vertex_descriptor v) {
        auto key = in_degree_key(v);
        auto res = _vertex_struct.search(key);
        if (res.status == ERR) {
            throw NotFound();
        }
        return res.value;
    }

    SpuUltraGraph::edges_size_type SpuUltraGraph::source_cnt(SpuUltraGraph::edge_descriptor e) {
        auto key = source_cnt_key(e);
        auto res = _edge_struct.search(key);
        if (res.status == ERR) {
            throw NotFound();
        }
        return res.value;
    }

    SpuUltraGraph::edges_size_type SpuUltraGraph::target_cnt(SpuUltraGraph::edge_descriptor e) {
        auto key = target_cnt_key(e);
        auto res = _edge_struct.search(key);
        if (res.status == ERR) {
            throw NotFound();
        }
        return res.value;
    }

    SpuUltraGraph::edge_descriptor SpuUltraGraph::get_edge_descriptor(id_t edge_id) {
        auto fields = Fields(_edge_id_fields_len);
        fields[EDGE_ID] = edge_id;
        fields[WEIGHT] = _graph_traits.default_weight;
        return data_t(fields);
    }

    SpuUltraGraph::edge_descriptor SpuUltraGraph::get_edge_descriptor(id_t edge_id, weight_t weight) {
        auto fields = Fields(_edge_id_fields_len);
        fields[EDGE_ID] = edge_id;
        fields[WEIGHT] = weight;
        return data_t(fields);
    }


    void SpuUltraGraph::ParallelEdgesIterator::increment() {
        auto from_key = _graph->vertex_key(_from, 0, _edge);
        auto to_key = _graph->vertex_key(_to, 1, _edge);

        auto resp = _graph->_vertex_struct.ngr(from_key);
        if (check_end(resp, _from, 0)) return;
        from_key = resp.key;

        resp = _graph->_vertex_struct.ngr(to_key);
        if (check_end(resp, _to, 1)) return;
        to_key = resp.key;

        while (from_key[EDGE_ID] != to_key[EDGE_ID]) {
            if (from_key[EDGE_ID] > to_key[EDGE_ID]) {
                to_key[EDGE_ID] = from_key[EDGE_ID];
                resp = _graph->_vertex_struct.ngr(to_key - 1);
                if (check_end(resp, _to, 1)) return;
                to_key = resp.key;
            } else {
                from_key[EDGE_ID] = to_key[EDGE_ID];
                resp = _graph->_vertex_struct.ngr(from_key - 1);
                if (check_end(resp, _from, 0)) return;
                from_key = resp.key;
            }
        }

        _edge = from_key[EDGE_ID];
    }

    void SpuUltraGraph::ParallelEdgesIterator::decrement() {
        auto from_key = _graph->vertex_key(_from, 0, _edge);
        auto to_key = _graph->vertex_key(_to, 1, _edge);

        auto resp = _graph->_vertex_struct.nsm(from_key);
        if (check_end(resp, _from, 0)) return;
        from_key = resp.key;

        resp = _graph->_vertex_struct.nsm(to_key);
        if (check_end(resp, _to, 1)) return;
        to_key = resp.key;

        while (from_key[EDGE_ID] != to_key[EDGE_ID]) {
            if (from_key[EDGE_ID] < to_key[EDGE_ID]) {
                to_key[EDGE_ID] = from_key[EDGE_ID];
                resp = _graph->_vertex_struct.nsm(to_key + 1);
                if (check_end(resp, _to, 1)) return;
                to_key = resp.key;
            } else {
                from_key[EDGE_ID] = to_key[EDGE_ID];
                resp = _graph->_vertex_struct.nsm(from_key + 1);
                if (check_end(resp, _from, 0)) return;
                from_key = resp.key;
            }
        }

        _edge = from_key[EDGE_ID];
    }

    SpuUltraGraph::edge_descriptor SpuUltraGraph::ParallelEdgesIterator::dereference() const {
        return _edge;
    }

    bool SpuUltraGraph::ParallelEdgesIterator::check_end(pair_t resp, SpuUltraGraph::vertex_descriptor vertex, uint8_t incidence)
    {
        auto key = _graph->vertex_key();
        key = resp.key;
        if (resp.status == ERR
            || (id_t) key[GRAPH_ID] != _graph->_graph_id
            || (id_t) key[VERTEX_ID] != vertex
            || (uint8_t) key[INCIDENCE] != incidence
            || !_graph->is_edge_id_valid(key[EDGE_ID])) {
            _edge = _graph->max_edge_id();
            return true;
        }
        return false;
    }



    void SpuUltraGraph::AdjacentEdgesIterator::increment() {
        auto key = _g->vertex_key(_v, _incidence, _edge);
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
        auto key = _g->vertex_key(_v, _incidence, _edge);
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