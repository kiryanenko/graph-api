//
// Created by kiryanenko on 10.05.19.
//

#ifndef GRAPH_API_SPUULTRAGRAPH_H
#define GRAPH_API_SPUULTRAGRAPH_H

#include <boost/graph/graph_traits.hpp>
#include <libspu.hpp>
#include <structure.hpp>
#include <memory>
#include "exceptions.h"
#include "GraphStructure.h"
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
        GraphStructure vertex_struct;
        GraphStructure edge_struct;

        size_t graph_id_depth = 3;
        size_t vertex_id_depth = 28;
        size_t edge_id_depth = 28;
        size_t weight_depth = 4;

        data_t default_vertex_value = 0;
        data_t default_edge_value = 0;
        data_t default_weight = 0;

        SpuUltraGraphTraits() {}
        SpuUltraGraphTraits(GraphStructure vertex_structure, GraphStructure edge_structure) :
                vertex_struct(vertex_structure), edge_struct(edge_structure) {}

        inline size_t depth_sum() { return graph_id_depth + vertex_id_depth + edge_id_depth + weight_depth + 1; }
    };



    class SpuUltraGraph {
        id_t _graph_id = 0;
        static id_t global_id_sequence;
        id_t _global_id;
        SpuUltraGraphTraits _graph_traits;

        typedef SPU::Fields<SPU_STRUCTURE_ATTRS> Fields;

        FieldsLength<SPU_STRUCTURE_ATTRS> _edge_id_fields_len;
        FieldsLength<SPU_STRUCTURE_ATTRS> _vertex_fields_len;
        FieldsLength<SPU_STRUCTURE_ATTRS> _edge_fields_len;

        GraphStructure _vertex_struct;
        GraphStructure _edge_struct;

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

        class VertexIterator :
                public iterator_facade<
                        VertexIterator,
                        vertex_descriptor,
                        bidirectional_traversal_tag,
                        vertex_descriptor>
        {
            friend class iterator_core_access;

            const SpuUltraGraph *_g;
            vertex_descriptor _v;

        public:
            VertexIterator() : _g(nullptr) {}
            VertexIterator(const SpuUltraGraph *g, vertex_descriptor v=0) : _g(g), _v(v) {}
            vertex_descriptor dereference() const { return _v; }
            bool equal(const VertexIterator& other) const { return _v == other._v; }
            void increment();
            void decrement();
        };

        /// Итератор по всем вершинам
        typedef VertexIterator vertex_iterator;

        /// Контейнер содержащий все ребра графа
        class Vertices
        {
            const SpuUltraGraph *_g;

        public:
            typedef vertex_iterator iterator;

            Vertices(const SpuUltraGraph *g) : _g(g) {}
            iterator begin() { iterator i(_g); return ++i; }
            iterator end() { return {_g, _g->max_vertex_id() + 1}; }
        };


        class EdgeIterator :
                public iterator_facade<
                        EdgeIterator,
                        edge_descriptor,
                        bidirectional_traversal_tag,
                        edge_descriptor>
        {
            friend class iterator_core_access;

            const SpuUltraGraph *_g;
            edge_descriptor _edge;

        public:
            EdgeIterator() : _g(nullptr) {}
            EdgeIterator(const SpuUltraGraph *g, edge_descriptor edge=0) : _g(g), _edge(edge) {}
            edge_descriptor dereference() const { return _edge; }
            bool equal(const EdgeIterator& other) const { return _edge == other._edge; }
            void increment();
            void decrement();
        };

        /// Итератор по всем ребрам
        typedef EdgeIterator edge_iterator;

        /// Контейнер содержащий все ребра графа
        class Edges
        {
            const SpuUltraGraph *_g;

        public:
            typedef edge_iterator iterator;

            Edges(const SpuUltraGraph *g) : _g(g) {}
            iterator begin() { iterator i(_g); return ++i; }
            iterator end() { return {_g, _g->max_edge_id() + 1}; }
        };



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
            edge_descriptor _edge;

        public:
            AdjacentEdgesIterator(const SpuUltraGraph *g, vertex_descriptor v, edge_descriptor edge=0) : _g(g), _v(v), _edge(edge) {}
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

            static AdjacentEdgesIterator begin(const SpuUltraGraph *g, vertex_descriptor v) { auto i = AdjacentEdgesIterator::rend(g, v); return ++i; }
            static AdjacentEdgesIterator end(const SpuUltraGraph *g, vertex_descriptor v) { return {g, v, g->max_edge_id() + 1}; }
            static AdjacentEdgesIterator rbegin(const SpuUltraGraph *g, vertex_descriptor v) { auto i = AdjacentEdgesIterator::end(g, v); return --i; }
            static AdjacentEdgesIterator rend(const SpuUltraGraph *g, vertex_descriptor v) { return {g, v}; }
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
            iterator begin() { return iterator::begin(_g, _v); }
            iterator end() { return iterator::end(_g, _v); }
        };


        class InEdges
        {
            const SpuUltraGraph *_g;
            vertex_descriptor _v;

        public:
            typedef in_edge_iterator iterator;

            InEdges(const SpuUltraGraph *g, SpuUltraGraph::vertex_descriptor v) : _g(g), _v(v) {}
            iterator begin() { return iterator::begin(_g, _v); }
            iterator end() { return iterator::end(_g, _v); }
        };



        /// Итератор по стокам для ребра e
        class TargetIterator :
                public iterator_facade<
                        TargetIterator,
                        vertex_descriptor,
                        bidirectional_traversal_tag,
                        vertex_descriptor>
        {
            friend class iterator_core_access;

            const SpuUltraGraph *_g;
            edge_descriptor _e;
            vertex_descriptor _v;

        public:
            TargetIterator(const SpuUltraGraph *g=nullptr, edge_descriptor e=0, vertex_descriptor v=0) : _g(g), _e(e), _v(v) {}
            vertex_descriptor dereference() const { return _v; }
            bool equal(const TargetIterator& other) const { return _v == other._v && _e == other._e; }
            void increment();
            void decrement();

            static TargetIterator begin(const SpuUltraGraph *g, edge_descriptor e) { auto i = TargetIterator::rend(g, e); return ++i; }
            static TargetIterator end(const SpuUltraGraph *g, edge_descriptor e) { return {g, e, g->max_vertex_id() + 1}; }
            static TargetIterator rbegin(const SpuUltraGraph *g, edge_descriptor e) { auto i = TargetIterator::end(g, e); return --i; }
            static TargetIterator rend(const SpuUltraGraph *g, edge_descriptor e) { return {g, e}; }
        };



        /// Итератор по вершинам смежным вершине v
        /// Итератор проходит по всем ребрам исходящих от v,
        /// т.о. при прохождении по параллельным ребрам будут повторяться вершины
        class AdjacentVerticesIterator :
                public iterator_facade<
                        AdjacentVerticesIterator,
                        vertex_descriptor,
                        bidirectional_traversal_tag,
                        vertex_descriptor>
        {
            friend class iterator_core_access;

            const SpuUltraGraph *_g;
            vertex_descriptor _v;
            out_edge_iterator _edge_iter;
            TargetIterator _target_iter;

        public:
            /// Конструктор итератора по вершинам смежным вершине v
            /// \param g Граф
            /// \param v Вершина относительно которой будет проходить итератор по смежным вершинам
            /// \param e Текущее ребро
            /// \param u Текущая вершина
            AdjacentVerticesIterator(const SpuUltraGraph *g=nullptr, vertex_descriptor v=0, edge_descriptor e=0, vertex_descriptor u=0) :
                _g(g), _v(v), _edge_iter(g, v, e), _target_iter(g, e, u) {}
            vertex_descriptor dereference() const { return *_target_iter; }
            bool equal(const AdjacentVerticesIterator& other) const { return _target_iter == other._target_iter; }
            void increment();
            void decrement();

            static AdjacentVerticesIterator begin(const SpuUltraGraph *g, vertex_descriptor v) { auto i = AdjacentVerticesIterator::rend(g, v); return ++i; }
            static AdjacentVerticesIterator end(const SpuUltraGraph *g, vertex_descriptor v) { return {g, v, g->max_edge_id() + 1, g->max_vertex_id() + 1}; }
            static AdjacentVerticesIterator rbegin(const SpuUltraGraph *g, vertex_descriptor v) { auto i = AdjacentVerticesIterator::end(g, v); return --i; }
            static AdjacentVerticesIterator rend(const SpuUltraGraph *g, vertex_descriptor v) { return {g, v}; }
        };

        /// Итератор по смежным вершинам
        typedef AdjacentVerticesIterator adjacency_iterator;

        /// Контейнер содержащий все ребра графа
        class AdjacentVertices
        {
            const SpuUltraGraph *_g;
            vertex_descriptor _v;

        public:
            typedef adjacency_iterator iterator;

            AdjacentVertices(const SpuUltraGraph *g, vertex_descriptor v) : _g(g), _v(v) {}
            iterator begin() { return iterator::begin(_g, _v); }
            iterator end() { return iterator::end(_g, _v); }
        };

        ////////////////////////////////////////////////////////



        explicit SpuUltraGraph(id_t graph_id = 0, const SpuUltraGraphTraits& spu_graph_traits = SpuUltraGraphTraits());

        /// Возвращает id графа, определенного в структурах
        id_t get_id() const { return _graph_id; }
        /// Возвращает глобальный id. Используется, например, для свойств графа.
        id_t get_global_id() const { return _global_id; }

        vertex_descriptor add_vertex();
        vertex_descriptor add_vertex(value_t value);
        vertex_descriptor add_vertex(vertex_descriptor id);
        vertex_descriptor add_vertex(vertex_descriptor id, value_t value);

        /// Добавляет вершину, если ее нет
        /// \param id Идентификатор вершины
        void put_vertex(vertex_descriptor id);
        /// Изменяет value у вершины. Если вершины нет, то добавляет новую.
        /// \param id Идентификатор вершины
        /// \param value Новый value
        void put_vertex(vertex_descriptor id, value_t value);

        void remove_vertex(vertex_descriptor v);

        bool has_vertex(vertex_descriptor id) const;

        vertices_size_type num_vertices() const;
        vertices_size_type degree(vertex_descriptor v) const { return out_degree(v) + in_degree(v); }
        vertices_size_type out_degree(vertex_descriptor v) const;
        vertices_size_type in_degree(vertex_descriptor v) const;

        /// Отсоединяет вершину v от всех ребер
        /// \param v Вершина
        /// \param remove_edges При true если у ребра не останется вершин "источников" или "стоков", то ребро будет удалено
        void clear_vertex(vertex_descriptor v, bool remove_edges=true);

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

        /// Добавляет ребро, если его нет
        /// \param id Идентификатор ребра
        void put_edge(edge_descriptor id);
        /// Изменяет value у ребра. Если ребра нет, то добавляет новое.
        /// \param id Идентификатор ребра
        /// \param value Новый value
        void put_edge(edge_descriptor id, value_t value);

        // Присоединяет как сток вершину vertex к ребру edge
        void add_target(edge_descriptor edge, vertex_descriptor vertex);
        // Присоединяет как источник вершину vertex к ребру edge
        void add_source(edge_descriptor edge, vertex_descriptor vertex);

        bool has_edge(edge_descriptor id) const;

        edge_descriptor get_edge_descriptor(id_t edge_id);
        /// Формирование edge_descriptor из веса ребра и индекса
        /// Это необходимо, например, для добавления взвешенных ребер.
        /// Индекс ребра это не id ребра. Как раз id ребра (edge_descriptor) состоит из веса и индекса.
        edge_descriptor get_edge_descriptor(id_t edge_id, weight_t weight);
        /// Получение веса ребра из edge_descriptor
        weight_t get_weight(edge_descriptor edge);

        edges_size_type num_edges() const;
        edges_size_type source_cnt(edge_descriptor e);
        edges_size_type target_cnt(edge_descriptor e);

        /// Возвращает первую вершину источник для ребра e
        /// Если источников, нет то вернет 0
        vertex_descriptor source(edge_descriptor e) const;
        /// Возвращает первую вершину сток для ребра e
        /// Если стоков нет, то вернет 0
        vertex_descriptor target(edge_descriptor e) const;

        void remove_edge(edge_descriptor edge);
        /// Удаляются все соединения от вершины from к to.
        /// Само ребро НЕ удаляется
        void remove_edge(vertex_descriptor from, vertex_descriptor to);

        /// Получить данные вершины
        /// \throw NotFound если нет вершины v
        value_t get_vertex_value(vertex_descriptor v) const;
        /// Получить данные ребра
        /// \throw NotFound если нет вершины e
        value_t get_edge_value(edge_descriptor e) const;

        /// Возвращает контейнер содержащий все вершины графа
        Vertices vertices() const { return {this}; }
        /// Возвращает контейнер содержащий вершины смежные вершине v
        AdjacentVertices adjacent_vertices(vertex_descriptor v) const { return {this, v}; }
        /// Возвращает контейнер содержащий все ребра графа
        Edges edges() const { return {this}; }
        /// Возвращает контейнер параллельных ребер от вершины from к to
        ParallelEdges parallel_edges(vertex_descriptor from, vertex_descriptor to) const;
        /// Возвращает контейнер исходящих ребер от вершины v
        OutEdges out_edges(vertex_descriptor v) const { return {this, v}; }
        /// Возвращает контейнер исходящих ребер от вершины v
        InEdges in_edges(vertex_descriptor v) const { return {this, v}; }

        /// Метод для отладки. Распечатывает структуру vertex
        void print_vertex_struct() const;
        /// Метод для отладки. Распечатывает структуру edge
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

        static id_t get_free_global_id() { return global_id_sequence++; }
        id_t get_free_edge_id();
        id_t get_free_edge_id(id_t min, id_t max) const;

        void inc_edges_cnt();
        void dec_edges_cnt();

        edges_size_type inc_target_cnt(edge_descriptor e, edges_size_type val = 1);
        edges_size_type dec_target_cnt(edge_descriptor e, edges_size_type val = 1);
        edges_size_type inc_source_cnt(edge_descriptor e, edges_size_type val = 1);
        edges_size_type dec_source_cnt(edge_descriptor e, edges_size_type val = 1);

        bool is_valid_edge_resp(pair_t resp, edge_descriptor edge, uint8_t incidence = 0) const;
    };
}


#endif //GRAPH_API_SPUULTRAGRAPH_H
