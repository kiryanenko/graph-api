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

        inline size_t depth_sum() { return graph_id_depth + vertex_id_depth + edge_id_depth + weight_id_depth; }
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

        //////////////////////////////////////////////////////////////

        explicit SpuUltraGraph(id_t graph_id = 0, SpuUltraGraphTraits spu_graph_traits = SpuUltraGraphTraits());
        ~SpuUltraGraph();

        vertex_descriptor add_vertex();
        vertex_descriptor add_vertex(value_t value);
        vertex_descriptor add_vertex(id_t id, bool safe=false);
        vertex_descriptor add_vertex(id_t id, value_t value, bool safe=false);

        bool has_vertex(id_t id);

        vertices_size_type num_vertices();

        edge_descriptor add_edge();
        edge_descriptor add_edge(id_t id, bool safe=false);
        edge_descriptor add_edge(vertex_descriptor u, vertex_descriptor v, bool safe=false);
        edge_descriptor add_edge(id_t id, vertex_descriptor u, vertex_descriptor v, bool safe=false);
        edge_descriptor add_weight_edge(weight_t weight);
        edge_descriptor add_weight_edge(id_t id, weight_t weight, bool safe=false);
        edge_descriptor add_weight_edge(vertex_descriptor u, vertex_descriptor v, weight_t weight, bool safe=false);
        edge_descriptor add_weight_edge(id_t id, vertex_descriptor u, vertex_descriptor v, weight_t weight, bool safe=false);
        edges_size_type num_edges();

    protected:
        Fields vertex_fields();
        Fields edge_fields();

        bool is_vertex_id_valid(id_t id);

        id_t get_free_vertex_id();
        id_t get_free_vertex_id(id_t min, id_t max);

        void inc_verteces_cnt();
        void dec_verteces_cnt();


        bool is_edge_id_valid(id_t id);

        id_t get_free_edge_id();
        id_t get_free_edge_id(id_t min, id_t max);

        void inc_edges_cnt();
        void dec_edges_cnt();
    };


    inline SpuUltraGraph::vertex_descriptor add_vertex(SpuUltraGraph &g) {
        return g.add_vertex();
    }
//
//    std::pair<SpuUltraGraph::edge_descriptor, bool> add_edge(SpuUltraGraph::vertex_descriptor u, SpuUltraGraph::vertex_descriptor v, SpuUltraGraph &g) {
//        std::pair<SpuUltraGraph::edge_descriptor, bool> p(u + v, true);
//        return p;
//    }
}

#endif //GRAPH_API_SPUULTRAGRAPH_H
