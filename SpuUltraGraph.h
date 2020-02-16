//
// Created by kiryanenko on 10.05.19.
//

#ifndef GRAPH_API_SPUULTRAGRAPH_H
#define GRAPH_API_SPUULTRAGRAPH_H

#include <boost/graph/graph_traits.hpp>
#include <libspu.hpp>
#include <structure.hpp>
#include "exceptions.h"
#include "StructureDecorator.h"
#include "StructureIterator.h"


#define KEY_DEPTH 64


namespace SPU_GRAPH
{
    using namespace SPU;
    using namespace boost;


    typedef unsigned long id_t;
    typedef unsigned long weight_t;


    enum SPU_STRUCTURE_ATTRS {
        GRAPH_ID,
        INCIDENCE,
        VERTEX_ID,
        EDGE_ID,
        WEIGHT
    };


    struct SpuUltraGraphTraits {
        size_t graph_id_depth = 3;
        size_t vertex_id_depth = 28;
        size_t edge_id_depth = 28;
        size_t weight_depth = 4;

        data_t default_vertex_value = 0;
        data_t default_edge_value = 0;
        data_t default_weight = 0;

        Structure<> *vertex_struct = nullptr;
        Structure<> *edge_struct = nullptr;

        inline size_t depth_sum() { return graph_id_depth + vertex_id_depth + edge_id_depth + weight_depth + 1; }
    };



    class SpuUltraGraph {
        id_t _graph_id = 0;
        SpuUltraGraphTraits _graph_traits;

        typedef SPU::Fields<SPU_STRUCTURE_ATTRS> Fields;

        FieldsLength<SPU_STRUCTURE_ATTRS> _edge_id_fields_len;
        FieldsLength<SPU_STRUCTURE_ATTRS> _vertex_fields_len;
        FieldsLength<SPU_STRUCTURE_ATTRS> _edge_fields_len;

        bool _should_free_vertex_struct = false;
        bool _should_free_edge_struct = false;

        StructureDecorator _vertex_struct;
        StructureDecorator _edge_struct;

        id_t _free_vertex_id = 1;
        id_t _free_edge_id = 1;

    public:
        /////////// Описание свойств графа для BOOST ////////////////

        // Тип объектов, используемых для идентификации вершин в графе
        typedef id_t vertex_descriptor;
        /// Тип объектов, используемых для идентификации ребер в графе.
        typedef id_t edge_descriptor;

        /// Категория обхода отражает поддерживаемые графовым классом виды итераторов
        struct spu_ultra_graph_traversal_category :
                public virtual bidirectional_graph_tag,
                public virtual adjacency_graph_tag,
                public virtual vertex_list_graph_tag,
                public virtual edge_list_graph_tag {};

        /// Предоставляет информацию о том, что граф ориентированный
        typedef directed_tag directed_category;
        /// Дает информацию о том, что граф позволяет вставку параллельных ребер
        /// (ребер, у которых одна и та же начальная и конечная вершины)
        typedef allow_parallel_edge_tag edge_parallel_category;
        /// Категория обхода отражает поддерживаемые графовым классом виды итераторов
        typedef spu_ultra_graph_traversal_category traversal_category;

        /// Тип, используемый для представления числа вершин в графе
        typedef size_t vertices_size_type;
        /// Тип, используемый для представления числа ребер в графе
        typedef size_t edges_size_type;
        /// Тип, используемый для представления числа исходящих ребер в графе
        typedef size_t degree_size_type;



        //////////////////// Итераторы и контейнеры /////////////////////////

        /// Итератор по параллельным ребрам от вершины from к to
        class ParallelEdgesIterator :
                public iterator_facade<
                        ParallelEdgesIterator,
                        edge_descriptor,
                        bidirectional_traversal_tag,
                        edge_descriptor>
        {
            friend class iterator_core_access;

            const SpuUltraGraph *_graph;
            vertex_descriptor _from, _to;
            id_t _edge;

        public:
            ParallelEdgesIterator(const SpuUltraGraph *g, vertex_descriptor from, vertex_descriptor to,
                    id_t edge=0) : _graph(g), _from(from), _to(to), _edge(edge) {}

            edge_descriptor dereference() const;
            bool equal(const ParallelEdgesIterator& other) const { return _edge == other._edge; }
            void increment();
            void decrement();

        private:
            bool check_end(pair_t resp, SpuUltraGraph::vertex_descriptor vertex, uint8_t incidence);
        };


        /// Контейнер параллельных ребер от вершины from к to
        class ParallelEdges
        {
            const SpuUltraGraph *_graph;
            vertex_descriptor _from, _to;

        public:
            typedef SpuUltraGraph::ParallelEdgesIterator iterator;

            ParallelEdges(const SpuUltraGraph *g, SpuUltraGraph::vertex_descriptor from,
                          SpuUltraGraph::vertex_descriptor v) : _graph(g), _from(from), _to(v) {}

            iterator begin() { iterator i(_graph, _from, _to); return ++i; }
            iterator end() { return {_graph, _from, _to, _graph->max_edge_id() + 1}; }
        };



        // Итератор по смежным ребрам
        template<int I>
        class AdjacentEdgesIterator :
                public iterator_facade<
                        AdjacentEdgesIterator<I>,
                        edge_descriptor,
                        bidirectional_traversal_tag,
                        edge_descriptor>
        {
            friend class iterator_core_access;

            const SpuUltraGraph *_g;
            vertex_descriptor _v;
            id_t _edge;

        public:
            AdjacentEdgesIterator(const SpuUltraGraph *g, vertex_descriptor v, id_t edge=0) : _g(g), _v(v), _edge(edge) {}
            edge_descriptor dereference() const { return _edge; }
            bool equal(const AdjacentEdgesIterator& other) const { return _edge == other._edge; }
            void increment() {
                auto key = _g->vertex_key(_v, I, _edge);
                auto resp = _g->_vertex_struct.ngr(key);
                key = resp.key;
                if (resp.status == ERR || (id_t) key[VERTEX_ID] != _v || (int) key[INCIDENCE] != I || !_g->is_edge_id_valid(key[EDGE_ID])) {
                    _edge = _g->max_edge_id() + 1;
                    return;
                }
                _edge = key[EDGE_ID];
            }
            void decrement() {
                auto key = _g->vertex_key(_v, I, _edge);
                auto resp = _g->_vertex_struct.nsm(key);
                key = resp.key;
                if (resp.status == ERR || (id_t) key[VERTEX_ID] != _v || (int) key[INCIDENCE] != I || !_g->is_edge_id_valid(key[EDGE_ID])) {
                    _edge = 0;
                    return;
                }
                _edge = key[EDGE_ID];
            }
        };

        /// Итератор по исходящим ребрам
        typedef AdjacentEdgesIterator<0> out_edge_iterator;
        /// Итератор по входящим ребрам
        typedef AdjacentEdgesIterator<1> in_edge_iterator;


        class OutEdges
        {
            const SpuUltraGraph *_g;
            vertex_descriptor _v;

        public:
            typedef out_edge_iterator iterator;

            OutEdges(const SpuUltraGraph *g, SpuUltraGraph::vertex_descriptor v) : _g(g), _v(v) {}
            iterator begin() { iterator i(_g, _v); return ++i; }
            iterator end() { return {_g, _v, _g->max_edge_id() + 1}; }
        };


        class InEdges
        {
            const SpuUltraGraph *_g;
            vertex_descriptor _v;

        public:
            typedef in_edge_iterator iterator;

            InEdges(const SpuUltraGraph *g, SpuUltraGraph::vertex_descriptor v) : _g(g), _v(v) {}
            iterator begin() { iterator i(_g, _v); return ++i; }
            iterator end() { return {_g, _v,_g->max_edge_id() + 1}; }
        };

        ////////////////////////////////////////////////////////



        explicit SpuUltraGraph(id_t graph_id = 0, SpuUltraGraphTraits spu_graph_traits = SpuUltraGraphTraits());
        ~SpuUltraGraph();

        vertex_descriptor add_vertex();
        vertex_descriptor add_vertex(value_t value);
        vertex_descriptor add_vertex(id_t id);
        vertex_descriptor add_vertex(id_t id, value_t value);

        void remove_vertex(vertex_descriptor v);

        bool has_vertex(vertex_descriptor id);

        vertices_size_type num_vertices() const;
        vertices_size_type out_degree(vertex_descriptor v) const;
        vertices_size_type in_degree(vertex_descriptor v) const;

        /// Отсоединяет вершину v от всех ребер
        void clear_vertex(vertex_descriptor v);

        // Отсоединят источник v от ребра e
        void disconnect_source(vertex_descriptor v, edge_descriptor e);
        // Отсоединят сток v от ребра e
        void disconnect_target(vertex_descriptor v, edge_descriptor e);

        edge_descriptor add_edge();
        edge_descriptor add_edge(value_t value);
        edge_descriptor add_edge(edge_descriptor id);
        edge_descriptor add_edge(edge_descriptor id, value_t value);
        edge_descriptor add_edge(vertex_descriptor from, vertex_descriptor to);
        edge_descriptor add_edge(edge_descriptor id, vertex_descriptor from, vertex_descriptor to);
        edge_descriptor add_edge(edge_descriptor id, vertex_descriptor from, vertex_descriptor to, value_t value);

        // Присоединяет как сток вершину vertex к ребру edge
        void add_target(edge_descriptor edge, vertex_descriptor vertex);
        // Присоединяет как источник вершину vertex к ребру edge
        void add_source(edge_descriptor edge, vertex_descriptor vertex);

        bool has_edge(edge_descriptor id);

        edge_descriptor get_edge_descriptor(id_t edge_id);
        edge_descriptor get_edge_descriptor(id_t edge_id, weight_t weight);
        weight_t get_weight(edge_descriptor edge);

        edges_size_type num_edges();
        edges_size_type source_cnt(edge_descriptor e);
        edges_size_type target_cnt(edge_descriptor e);

        void remove_edge(edge_descriptor edge);
        /// Удаляются все соединения от вершины from к to.
        /// Само ребро НЕ удаляется
        void remove_edge(vertex_descriptor from, vertex_descriptor to);

        /// Возвращает контейнер параллельных ребер от вершины from к to
        ParallelEdges parallel_edges(vertex_descriptor from, vertex_descriptor to) const;
        /// Возвращает контейнер исходящих ребер от вершины v
        OutEdges out_edges(vertex_descriptor v) const { return {this, v}; }
        /// Возвращает контейнер исходящих ребер от вершины v
        InEdges in_edges(vertex_descriptor v) const { return {this, v}; }

        void print_vertex_struct() const;
        void print_edge_struct() const;

    protected:
        Fields vertex_key(id_t vertex = 0, uint8_t incidence = 0, id_t edge = 0) const;
        Fields edge_key(id_t edge = 0, uint8_t incidence = 0, id_t vertex = 0) const;
        Fields out_degree_key(vertex_descriptor v) const;
        Fields in_degree_key(vertex_descriptor v) const;
        Fields target_cnt_key(edge_descriptor e) const;
        Fields source_cnt_key(edge_descriptor e) const;
        id_t max_vertex_id() const;
        id_t max_edge_id() const;
        id_t max_weight() const;


        bool is_vertex_id_valid(id_t id) const;

        id_t get_free_vertex_id();
        id_t get_free_vertex_id(id_t min, id_t max) const;

        vertices_size_type inc_vertexes_cnt();
        vertices_size_type dec_vertexes_cnt();

        vertices_size_type inc_out_degree(vertex_descriptor v, vertices_size_type val = 1);
        vertices_size_type dec_out_degree(vertex_descriptor v, vertices_size_type val = 1);
        vertices_size_type inc_in_degree(vertex_descriptor v, vertices_size_type val = 1);
        vertices_size_type dec_in_degree(vertex_descriptor v, vertices_size_type val = 1);

        vertex_descriptor _add_vertex(vertex_descriptor id, value_t value);
        edge_descriptor _add_edge(edge_descriptor id, value_t value);
        void _add_target(edge_descriptor edge, vertex_descriptor vertex);
        void _add_source(edge_descriptor edge, vertex_descriptor vertex);

        bool is_edge_id_valid(id_t id) const;

        id_t get_free_edge_id();
        id_t get_free_edge_id(id_t min, id_t max) const;

        void inc_edges_cnt();
        void dec_edges_cnt();

        edges_size_type inc_target_cnt(edge_descriptor e, edges_size_type val = 1);
        edges_size_type dec_target_cnt(edge_descriptor e, edges_size_type val = 1);
        edges_size_type inc_source_cnt(edge_descriptor e, edges_size_type val = 1);
        edges_size_type dec_source_cnt(edge_descriptor e, edges_size_type val = 1);
    };
}


#endif //GRAPH_API_SPUULTRAGRAPH_H
