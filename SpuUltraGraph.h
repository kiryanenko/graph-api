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
        size_t weight_id_depth = 4;

        data_t default_vertex_value = 0;
        data_t default_weight = 0;

        Structure<> *vertex_struct = nullptr;
        Structure<> *edge_struct = nullptr;

        inline size_t depth_sum() { return graph_id_depth + vertex_id_depth + edge_id_depth + weight_id_depth + 1; }
    };



    class SpuUltraGraph {
        id_t _graph_id = 0;
        SpuUltraGraphTraits _graph_traits;

        typedef SPU::Fields<SPU_STRUCTURE_ATTRS> Fields;

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
                public virtual edge_list_graph_tag { };

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

        class ParallelEdgesIterator :
                public iterator_facade<
                        ParallelEdgesIterator,
                        std::pair<edge_descriptor, weight_t>,
                        bidirectional_traversal_tag,
                        std::pair<edge_descriptor, weight_t>>
        {
            friend class iterator_core_access;

            const SpuUltraGraph *_graph;
            vertex_descriptor _u, _v;
            id_t _edge;
            weight_t _weight;

        public:
            ParallelEdgesIterator(const SpuUltraGraph *g, vertex_descriptor u, vertex_descriptor v,
                    id_t edge=0, weight_t weight=0) : _graph(g), _u(u), _v(v), _edge(edge), _weight(weight) {}

            std::pair<edge_descriptor, weight_t> dereference() const;
            bool equal(const ParallelEdgesIterator& other) const { return _edge == other._edge && _weight == other._weight; }
            void increment();
            void decrement();
        };


        class ParallelEdges
        {
            const SpuUltraGraph *_graph;
            vertex_descriptor _u, _v;

        public:
            typedef SpuUltraGraph::ParallelEdgesIterator iterator;

            ParallelEdges(const SpuUltraGraph *g, SpuUltraGraph::vertex_descriptor u,
                          SpuUltraGraph::vertex_descriptor v) : _graph(g), _u(u), _v(v) {}

            iterator begin() { iterator i(_graph, _u, _v); return ++i; }
            iterator end() { return {_graph, _u, _v, 0}; }
        };



        class AdjacentEdgesIterator :
                public iterator_facade<
                        AdjacentEdgesIterator,
                        std::pair<edge_descriptor, weight_t>,
                        bidirectional_traversal_tag,
                        std::pair<edge_descriptor, weight_t>>
        {
            friend class iterator_core_access;

            const SpuUltraGraph *_g;
            uint8_t _incidence;
            vertex_descriptor _v;
            id_t _edge;
            weight_t _weight;

        public:
            AdjacentEdgesIterator(const SpuUltraGraph *g, uint8_t incidence = 0, vertex_descriptor v = 0,
                    id_t edge=0, weight_t weight=0) : _g(g), _incidence(incidence), _v(v), _edge(edge), _weight(weight) {}

            std::pair<edge_descriptor, weight_t> dereference() const { return {_edge, _weight}; }
            bool equal(const AdjacentEdgesIterator& other) const;
            void increment();
            void decrement();
        };


        class OutEdges
        {
            const SpuUltraGraph *_g;
            vertex_descriptor _v;

        public:
            typedef SpuUltraGraph::AdjacentEdgesIterator iterator;

            OutEdges(const SpuUltraGraph *g, SpuUltraGraph::vertex_descriptor u,
                     SpuUltraGraph::vertex_descriptor v) : _g(g), _v(v) {}

            iterator begin() { iterator i(_g, 1, _v); return ++i; }
            iterator end() { return {_g, 1, _v,
                                     _g->_vertex_fields_len.fieldMask(EDGE_ID),
                                     _g->_vertex_fields_len.fieldMask(WEIGHT)}; }
        };


        class InEdges
        {
            const SpuUltraGraph *_g;
            vertex_descriptor _v;

        public:
            typedef SpuUltraGraph::AdjacentEdgesIterator iterator;

            InEdges(const SpuUltraGraph *g, SpuUltraGraph::vertex_descriptor u,
                    SpuUltraGraph::vertex_descriptor v) : _g(g), _v(v) {}

            iterator begin() { iterator i(_g, 0, _v); return ++i; }
            iterator end() { return {_g, 0, _v,
                                     _g->_vertex_fields_len.fieldMask(EDGE_ID),
                                     _g->_vertex_fields_len.fieldMask(WEIGHT)}; }
        };

        ////////////////////////////////////////////////////////



        explicit SpuUltraGraph(id_t graph_id = 0, SpuUltraGraphTraits spu_graph_traits = SpuUltraGraphTraits());
        ~SpuUltraGraph();

        vertex_descriptor add_vertex();
        vertex_descriptor add_vertex(value_t value);
        vertex_descriptor add_vertex(id_t id, bool safe=false);
        vertex_descriptor add_vertex(id_t id, value_t value, bool safe=false);

        bool has_vertex(id_t id);

        vertices_size_type num_vertices();

        void clear_vertex(vertex_descriptor v);


        edge_descriptor add_edge();
        edge_descriptor add_edge(id_t id, bool safe=false);
        edge_descriptor add_edge(vertex_descriptor u, vertex_descriptor v, bool safe=false);
        edge_descriptor add_edge(id_t id, vertex_descriptor u, vertex_descriptor v, bool safe=false);
        edge_descriptor add_weight_edge(weight_t weight);
        edge_descriptor add_weight_edge(id_t id, weight_t weight, bool safe=false);
        edge_descriptor add_weight_edge(vertex_descriptor u, vertex_descriptor v, weight_t weight, bool safe=false);
        edge_descriptor add_weight_edge(id_t id, vertex_descriptor u, vertex_descriptor v, weight_t weight, bool safe=false);

        void add_target(edge_descriptor edge, vertex_descriptor vertex, bool safe=false);
        void add_source(edge_descriptor edge, vertex_descriptor vertex, bool safe=false);

        bool has_edge(id_t id);
        weight_t get_weight(edge_descriptor edge);

        edges_size_type num_edges();

        void remove_edge(edge_descriptor edge);
        void remove_edge(vertex_descriptor u, vertex_descriptor v);

        ParallelEdges parallel_edges(vertex_descriptor u, vertex_descriptor v) const;

    protected:
        Fields vertex_fields(id_t vertex = 0, uint8_t incidence=0, weight_t weight=0, id_t edge=0) const;
        Fields edge_fields(id_t edge = 0, uint8_t incidence=0, id_t vertex=0) const;

        bool is_vertex_id_valid(id_t id) const;

        id_t get_free_vertex_id();
        id_t get_free_vertex_id(id_t min, id_t max) const;

        void inc_verteces_cnt();
        void dec_verteces_cnt();

        void inc_out_degree(vertex_descriptor v, size_t val = 1);
        void dec_out_degree(vertex_descriptor v, size_t val = 1);
        void inc_in_degree(vertex_descriptor v, size_t val = 1);
        void dec_in_degree(vertex_descriptor v, size_t val = 1);


        bool is_edge_id_valid(id_t id) const;

        id_t get_free_edge_id();
        id_t get_free_edge_id(id_t min, id_t max) const;

        void inc_edges_cnt();
        void dec_edges_cnt();

        void inc_target_cnt(edge_descriptor e, size_t val = 1);
        void dec_target_cnt(edge_descriptor e, size_t val = 1);
        void inc_source_cnt(edge_descriptor e, size_t val = 1);
        void dec_source_cnt(edge_descriptor e, size_t val = 1);

        void add_target(edge_descriptor edge, vertex_descriptor vertex, weight_t weight);
        void add_source(edge_descriptor edge, vertex_descriptor vertex, weight_t weight);
    };


    inline SpuUltraGraph::vertex_descriptor add_vertex(SpuUltraGraph &g) {
        return g.add_vertex();
    }

    inline std::pair<SpuUltraGraph::edge_descriptor, bool>
    add_edge(SpuUltraGraph::vertex_descriptor u, SpuUltraGraph::vertex_descriptor v, SpuUltraGraph &g) {
        return {g.add_edge(u, v), true};
    }


    inline void remove_edge(SpuUltraGraph::vertex_descriptor u, SpuUltraGraph::vertex_descriptor v, SpuUltraGraph &g) {
        g.remove_edge(u, v);
    }

    inline void remove_edge(SpuUltraGraph::edge_descriptor edge, SpuUltraGraph &g) {
        g.remove_edge(edge);
    }
}

#endif //GRAPH_API_SPUULTRAGRAPH_H
