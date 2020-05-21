//
// Created by kiryanenko on 10.05.19.
//

#include "SpuUltraGraph.h"
#include <iostream>
#include <cstdio>
#include "utils.h"


using namespace std;


namespace SPU_GRAPH
{
    id_t SpuUltraGraph::global_id_sequence = 0;

    SpuUltraGraph::SpuUltraGraph(SPU_GRAPH::id_t graph_id, const SPU_GRAPH::SpuUltraGraphTraits& spu_graph_traits) :
            _graph_id(graph_id),
            _global_id(get_free_global_id()),
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
                             }),
            _vertex_struct(spu_graph_traits.vertex_struct),
            _edge_struct(spu_graph_traits.edge_struct)
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
    }

    SpuUltraGraph::vertex_descriptor SpuUltraGraph::add_vertex() {
        return add_vertex(_graph_traits.default_vertex_value);
    }

    SpuUltraGraph::vertex_descriptor SpuUltraGraph::add_vertex(value_t value) {
        auto id = get_free_vertex_id();
        return _add_vertex(id, value);
    }

    SpuUltraGraph::vertex_descriptor SpuUltraGraph::add_vertex(id_t id) {
        return _add_vertex(id, _graph_traits.default_vertex_value);
    }

    SpuUltraGraph::vertex_descriptor SpuUltraGraph::add_vertex(id_t id, value_t value) {
        if (!is_vertex_id_valid(id)) {
            throw BadRequest("Invalid ID");
        }
        if (has_vertex(id)) {
            throw Conflict();
        }
        return _add_vertex(id, value);
    }

    SpuUltraGraph::vertex_descriptor SpuUltraGraph::_add_vertex(SpuUltraGraph::vertex_descriptor id, value_t value) {
        _vertex_struct.insert(vertex_key(id), value);
        _vertex_struct.insert(out_degree_key(id), 0);
        _vertex_struct.insert(in_degree_key(id), 0);

        inc_vertexes_cnt();
        return id;
    }

    void SpuUltraGraph::put_vertex(SpuUltraGraph::vertex_descriptor id) {
        if (!has_vertex(id)) {
            _add_vertex(id, _graph_traits.default_vertex_value);
        }
    }

    void SpuUltraGraph::put_vertex(SpuUltraGraph::vertex_descriptor id, value_t value) {
        if (!has_vertex(id)) {
            _add_vertex(id, value);
        } else {
            _vertex_struct.insert(vertex_key(id), value);
        }
    }


    SpuUltraGraph::vertices_size_type SpuUltraGraph::num_vertices() const {
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

        id_t id = get_free_domain(_vertex_struct, _vertex_fields_len[VERTEX_ID],
                                  _graph_id << 1, _graph_traits.graph_id_depth + 1, 1, max_vertex_id());
        _free_vertex_id = id + 1;
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
        return _add_edge(id, _graph_traits.default_edge_value);
    }

    SpuUltraGraph::edge_descriptor SpuUltraGraph::add_edge(value_t value) {
        auto id = get_free_edge_id();
        return _add_edge(id, value);
    }

    SpuUltraGraph::edge_descriptor SpuUltraGraph::add_edge(id_t id) {
        return add_edge(id, _graph_traits.default_edge_value);
    }

    SpuUltraGraph::edge_descriptor SpuUltraGraph::add_edge(id_t id, value_t value) {
        if (!is_edge_id_valid(id)) {
            throw BadRequest("Invalid ID");
        }
        if (has_edge(id)) {
            throw Conflict();
        }
        return _add_edge(id, value);
    }

    SpuUltraGraph::edge_descriptor SpuUltraGraph::_add_edge(SpuUltraGraph::edge_descriptor id, value_t value) {
        _edge_struct.insert(edge_key(id), value);
        _edge_struct.insert(source_cnt_key(id), 0);
        _edge_struct.insert(target_cnt_key(id), 0);

        inc_edges_cnt();
        return id;
    }

    SpuUltraGraph::edge_descriptor
    SpuUltraGraph::add_edge(SpuUltraGraph::vertex_descriptor from, SpuUltraGraph::vertex_descriptor to) {
        auto id = get_free_edge_id();
        if ((!has_vertex(from) || !has_vertex(to))) {
            throw NotFound();
        }
        _add_edge(id, _graph_traits.default_edge_value);
        _add_source(id, from);
        _add_target(id, to);
        return id;
    }

    SpuUltraGraph::edge_descriptor
    SpuUltraGraph::add_edge(SpuUltraGraph::edge_descriptor id, SpuUltraGraph::vertex_descriptor from, SpuUltraGraph::vertex_descriptor to) {
        return add_edge(id, from, to, _graph_traits.default_edge_value);
    }

    SpuUltraGraph::edge_descriptor
    SpuUltraGraph::add_edge(SpuUltraGraph::edge_descriptor id, SpuUltraGraph::vertex_descriptor from, SpuUltraGraph::vertex_descriptor to,
                            value_t value) {
        if ((!has_vertex(from) || !has_vertex(to))) {
            throw NotFound();
        }
        add_edge(id, value);
        _add_source(id, from);
        _add_target(id, to);
        return id;
    }

    void SpuUltraGraph::put_edge(SpuUltraGraph::edge_descriptor id) {
        if (!has_edge(id)) {
            _add_edge(id, _graph_traits.default_edge_value);
        }
    }

    void SpuUltraGraph::put_edge(SpuUltraGraph::edge_descriptor id, value_t value) {
        if (!has_edge(id)) {
            _add_edge(id, value);
        } else {
            _edge_struct.insert(edge_key(id), value);
        }
    }


    // Количество ребер в графе
    SpuUltraGraph::edges_size_type SpuUltraGraph::num_edges() const {
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

        id_t id = get_free_domain(_edge_struct, _edge_fields_len[EDGE_ID],
                _graph_id << 1, _graph_traits.graph_id_depth + 1, 1, max_edge_id());
        _free_edge_id = id + 1;
        return id;
    }

    // Возвращает свободный дискриптор ребра с определенным весом, для последующего добавления
    SpuUltraGraph::edge_descriptor SpuUltraGraph::get_free_edge_descriptor(weight_t weight) const {
        auto prefix = ((SPU::key_t) _graph_id << u8(1 + _graph_traits.weight_depth)) + weight;
        auto prefix_depth = _graph_traits.graph_id_depth + 1 + _graph_traits.weight_depth;
        auto max = FieldsLength<int>::mask(_graph_traits.edge_id_depth) - 1;
        id_t edge_id = get_free_domain(_edge_struct, _graph_traits.edge_id_depth,
                                       prefix, prefix_depth, 1, max);
        return get_edge_descriptor(edge_id, weight);
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

    bool SpuUltraGraph::has_vertex(vertex_descriptor id) const {
        auto key = vertex_key(id);
        auto res = _vertex_struct.search(key);
        return res.status == OK;
    }

    bool SpuUltraGraph::has_edge(edge_descriptor id) const {
        auto key = edge_key(id);
        auto res = _edge_struct.search(key);
        return res.status == OK;
    }

    void
    SpuUltraGraph::add_target(SpuUltraGraph::edge_descriptor edge, SpuUltraGraph::vertex_descriptor vertex) {
        if (!has_vertex(vertex) || !has_vertex(edge)) {
            throw NotFound();
        }
        _add_target(edge, vertex);
    }

    void
    SpuUltraGraph::_add_target(SpuUltraGraph::edge_descriptor edge, SpuUltraGraph::vertex_descriptor target) {
        auto v_key = vertex_key(target, 1, edge);
        _vertex_struct.insert(v_key);
        auto e_key = edge_key(edge, 1, target);
        _edge_struct.insert(e_key);

        inc_in_degree(target);
        inc_target_cnt(edge);
    }

    void
    SpuUltraGraph::add_source(SpuUltraGraph::edge_descriptor edge, SpuUltraGraph::vertex_descriptor vertex) {
        if (!has_vertex(vertex) || !has_vertex(edge)) {
            throw NotFound();
        }
        _add_source(edge, vertex);
    }

    void
    SpuUltraGraph::_add_source(SpuUltraGraph::edge_descriptor edge, SpuUltraGraph::vertex_descriptor vertex) {
        auto v_key = vertex_key(vertex, 0, edge);
        _vertex_struct.insert(v_key);
        auto e_key = edge_key(edge, 0, vertex);
        _edge_struct.insert(e_key);

        inc_out_degree(vertex);
        inc_source_cnt(edge);
    }

    weight_t SpuUltraGraph::get_weight(SpuUltraGraph::edge_descriptor edge) const {
        auto fields = Fields(_edge_id_fields_len);
        fields = edge;
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
    /// Ребро удаляется, если кол-во вершин источников или стоков равно 0
    void SpuUltraGraph::remove_edge(SpuUltraGraph::vertex_descriptor from, SpuUltraGraph::vertex_descriptor to) {
        auto from_vertex_key = vertex_key(from);
        auto to_vertex_key = vertex_key(to, 1);
        auto from_edge_key = edge_key(0, 0, from);
        auto to_edge_key = edge_key(0, 1, to);
        for (auto edge_id : parallel_edges(from, to)) {
            disconnect_source(from, edge_id);
            disconnect_target(from, edge_id);
            if (source_cnt(edge_id) == 0 || target_cnt(edge_id) == 0) {
                remove_edge(edge_id);
            }
        }
    }

    SpuUltraGraph::ParallelEdges
    SpuUltraGraph::parallel_edges(SpuUltraGraph::vertex_descriptor from, SpuUltraGraph::vertex_descriptor to) const {
        return {this, from, to};
    }

    /// Отсоединяет вершину v от всех ребер
    /// Если после ребро не соединено ни с одной вершиной, то оно удаляется
    void SpuUltraGraph::clear_vertex(SpuUltraGraph::vertex_descriptor v, bool remove_edges) {
        for (auto e : out_edges(v)) {
            disconnect_source(v, e);
            if (remove_edges && source_cnt(e) == 0) {
                remove_edge(e);
            }
        }
        for (auto e : in_edges(v)) {
            disconnect_target(v, e);
            if (remove_edges && target_cnt(e) == 0) {
                remove_edge(e);
            }
        }
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
        _edge_struct.insert(target_cnt_key(e), cnt);
        return cnt;
    }

    SpuUltraGraph::edges_size_type SpuUltraGraph::dec_target_cnt(SpuUltraGraph::edge_descriptor e, SpuUltraGraph::edges_size_type val) {
        auto cnt = target_cnt(e) - val;
        _edge_struct.insert(target_cnt_key(e), cnt);
        return cnt;
    }

    SpuUltraGraph::edges_size_type SpuUltraGraph::inc_source_cnt(SpuUltraGraph::edge_descriptor e, SpuUltraGraph::edges_size_type val) {
        auto cnt = source_cnt(e) + val;
        _edge_struct.insert(source_cnt_key(e), cnt);
        return cnt;
    }

    SpuUltraGraph::edges_size_type SpuUltraGraph::dec_source_cnt(SpuUltraGraph::edge_descriptor e, SpuUltraGraph::edges_size_type val) {
        auto cnt = source_cnt(e) - val;
        _edge_struct.insert(source_cnt_key(e), cnt);
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

    // Возвращает количество исходящих ребер
    SpuUltraGraph::vertices_size_type SpuUltraGraph::out_degree(SpuUltraGraph::vertex_descriptor v) const {
        auto key = out_degree_key(v);
        auto res = _vertex_struct.search(key);
        if (res.status == ERR) {
            throw NotFound();
        }
        return res.value;
    }

    // Возвращает количество входящих ребер.
    SpuUltraGraph::vertices_size_type SpuUltraGraph::in_degree(SpuUltraGraph::vertex_descriptor v) const {
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

    SpuUltraGraph::edge_descriptor SpuUltraGraph::get_edge_descriptor(id_t edge_id) const {
        auto fields = Fields(_edge_id_fields_len);
        fields[EDGE_ID] = edge_id;
        fields[WEIGHT] = _graph_traits.default_weight;
        data_t data = fields;
        return data;
    }

    SpuUltraGraph::edge_descriptor SpuUltraGraph::get_edge_descriptor(id_t edge_id, weight_t weight) const {
        auto fields = Fields(_edge_id_fields_len);
        fields[EDGE_ID] = edge_id;
        fields[WEIGHT] = weight;
        data_t data = fields;
        return data;
    }

    void SpuUltraGraph::disconnect_source(SpuUltraGraph::vertex_descriptor v, SpuUltraGraph::edge_descriptor e) {
        auto v_key = vertex_key(v, 0, e);
        if (_vertex_struct.search(v_key).status == OK) {
            _vertex_struct.del(v_key);
            dec_out_degree(v);
        }
        auto e_key = edge_key(e, 0, v);
        if (_edge_struct.search(e_key).status == OK) {
            _edge_struct.del(e_key);
            dec_source_cnt(e);
        }
    }

    void SpuUltraGraph::disconnect_target(SpuUltraGraph::vertex_descriptor v, SpuUltraGraph::edge_descriptor e) {
        auto v_key = vertex_key(v, 1, e);
        if (_vertex_struct.search(v_key).status == OK) {
            _vertex_struct.del(v_key);
            dec_in_degree(v);
        }
        auto e_key = edge_key(e, 1, v);
        if (_edge_struct.search(e_key).status == OK) {
            _edge_struct.del(e_key);
            dec_target_cnt(e);
        }
    }

    // Удаляет вершину v из графа
    void SpuUltraGraph::remove_vertex(SpuUltraGraph::vertex_descriptor v) {
        if (!has_vertex(v)) return;
        clear_vertex(v);
        _vertex_struct.del(vertex_key(v));
        _vertex_struct.del(out_degree_key(v));
        _vertex_struct.del(in_degree_key(v));
        dec_vertexes_cnt();
    }

    // Возвращает источник для ребра e
    SpuUltraGraph::vertex_descriptor SpuUltraGraph::source(SpuUltraGraph::edge_descriptor e) const {
        auto key = edge_key(e);
        auto resp = _edge_struct.ngr(key);
        key = resp.key;
        auto vertex_id = key[VERTEX_ID];
        if (!is_valid_edge_resp(resp, e) || !is_vertex_id_valid(vertex_id)) {
            return 0;
        }
        return vertex_id;
    }

    SpuUltraGraph::vertex_descriptor SpuUltraGraph::target(SpuUltraGraph::edge_descriptor e) const {
        auto key = edge_key(e, 1);
        auto resp = _edge_struct.ngr(key);
        key = resp.key;
        auto vertex_id = key[VERTEX_ID];
        if (!is_valid_edge_resp(resp, e, 1) || !is_vertex_id_valid(vertex_id)) {
            return 0;
        }
        return vertex_id;
    }

    bool SpuUltraGraph::is_valid_edge_resp(pair_t resp, edge_descriptor e, uint8_t incidence) const {
        auto key = edge_key();
        key = resp.key;
        return resp.status == OK
               && (edge_descriptor) key[EDGE_ID] == e
               && (int) key[INCIDENCE] == incidence
               && (id_t) key[GRAPH_ID] == _graph_id;
    }

    value_t SpuUltraGraph::get_vertex_value(vertex_descriptor v) const {
        auto resp = _vertex_struct.search(vertex_key(v));
        if (resp.status == ERR) {
            throw NotFound();
        }
        return resp.value;
    }

    value_t SpuUltraGraph::get_edge_value(SpuUltraGraph::edge_descriptor e) const {
        auto resp = _edge_struct.search(edge_key(e));
        if (resp.status == ERR) {
            throw NotFound();
        }
        return resp.value;
    }

    SpuUltraGraph::edge_descriptor SpuUltraGraph::edge(vertex_descriptor from, vertex_descriptor to) const {
        auto edges = parallel_edges(from, to);
        auto iter = edges.begin();
        if (iter == edges.end()) {
            return 0;
        }
        return *iter;
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
            _edge = _graph->max_edge_id() + 1;
            return true;
        }
        return false;
    }



    void SpuUltraGraph::EdgeIterator::increment() {
        auto key = _g->edge_key(_edge  + 1);
        auto resp = _g->_edge_struct.ngr(key - 1);
        key = resp.key;
        if (resp.status == ERR
            || (int) key[INCIDENCE] != 0
            || !_g->is_edge_id_valid(key[EDGE_ID])
            || (id_t) key[GRAPH_ID] != _g->_graph_id) {
            _edge = _g->max_edge_id() + 1;
            return;
        }
        _edge = key[EDGE_ID];
    }

    void SpuUltraGraph::EdgeIterator::decrement() {
        auto key = _g->edge_key(_edge);
        auto resp = _g->_edge_struct.nsm(key);
        key = resp.key;
        if (resp.status == ERR
            || (int) key[INCIDENCE] != 0
            || !_g->is_edge_id_valid(key[EDGE_ID])
            || (id_t) key[GRAPH_ID] != _g->_graph_id) {
            _edge = 0;
            return;
        }
        _edge = key[EDGE_ID];
    }

    void SpuUltraGraph::VertexIterator::increment() {
        auto key = _g->vertex_key(_v + 1);
        auto resp = _g->_vertex_struct.ngr(key - 1);
        key = resp.key;
        auto vertex_id = key[VERTEX_ID];
        if (resp.status == ERR
            || (int) key[INCIDENCE] != 0
            || !_g->is_vertex_id_valid(vertex_id)
            || (id_t) key[GRAPH_ID] != _g->_graph_id) {
            _v = _g->max_vertex_id() + 1;
            return;
        }
        _v = vertex_id;
    }

    void SpuUltraGraph::VertexIterator::decrement() {
        auto key = _g->vertex_key(_v);
        auto resp = _g->_vertex_struct.nsm(key);
        key = resp.key;
        auto vertex_id = key[VERTEX_ID];
        if (resp.status == ERR
            || (int) key[INCIDENCE] != 0
            || !_g->is_vertex_id_valid(vertex_id)
            || (id_t) key[GRAPH_ID] != _g->_graph_id) {
            _v = 0;
            return;
        }
        _v = vertex_id;
    }


    void SpuUltraGraph::AdjacentVerticesIterator::increment() {
        if (_edge_iter == OutEdges(_g, _v).rend()) {
            ++_edge_iter;
            _target_iter = Targets(_g, *_edge_iter).begin();
            return;
        }
        ++_target_iter;
        while (_target_iter == Targets(_g, *_edge_iter).end()) {
            ++_edge_iter;
            if (_edge_iter == OutEdges(_g, _v).end()) {
                _target_iter = Targets(_g, *_edge_iter).end();
                return;
            }
            _target_iter = Targets(_g, *_edge_iter).begin();
        }
    }

    void SpuUltraGraph::AdjacentVerticesIterator::decrement() {
        if (_edge_iter == OutEdges(_g, _v).end()) {
            --_edge_iter;
            _target_iter = Targets(_g, *_edge_iter).rbegin();
            return;
        }
        --_target_iter;
        while (_target_iter == Targets(_g, *_edge_iter).rend()) {
            --_edge_iter;
            if (_edge_iter == OutEdges(_g, _v).rend()) {
                _target_iter = Targets(_g, *_edge_iter).rend();
                return;
            }
            _target_iter = Targets(_g, *_edge_iter).rbegin();
        }
    }
}