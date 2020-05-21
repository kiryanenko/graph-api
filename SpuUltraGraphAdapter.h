//
// Created by a.kiryanenko on 2/16/20.
//

#ifndef GRAPH_API_SPUULTRAGRAPHADAPTER_H
#define GRAPH_API_SPUULTRAGRAPHADAPTER_H

#include "SpuUltraGraph.h"

namespace boost {
    using namespace SPU_GRAPH;


    // Концепция MutableGraph
    // =============================================

    /// Добавляет вершину в граф g.
    /// \return Дескриптор добавленной вершины
    inline SpuUltraGraph::vertex_descriptor add_vertex(SpuUltraGraph &g) { return g.add_vertex(); }
    /// Вставляет ребро в граф g между вершинами u и v.
    /// Если граф запрещает параллельные ребра, то флаг устанавливается в значение false.
    /// \return Дескриптор добавленного ребра
    inline std::pair<SpuUltraGraph::edge_descriptor, bool>
    add_edge(SpuUltraGraph::vertex_descriptor from, SpuUltraGraph::vertex_descriptor to, SpuUltraGraph &g) {
        return {g.add_edge(from, to), true};
    }
    /// Удаляет все ребра между вершинами.
    inline void remove_edge(SpuUltraGraph::vertex_descriptor u, SpuUltraGraph::vertex_descriptor v, SpuUltraGraph &g) { g.remove_edge(u, v); }
    /// Удаляет ребро edge из графа g.
    inline void remove_edge(SpuUltraGraph::edge_descriptor edge, SpuUltraGraph &g) { g.remove_edge(edge); }
    /// Удаляет все ребра смежные с вершиной.
    inline void clear_vertex(SpuUltraGraph::vertex_descriptor v, SpuUltraGraph &g) { g.clear_vertex(v);}
    /// Удаляет вершину v из графа g.
    inline void remove_vertex(SpuUltraGraph::vertex_descriptor v, SpuUltraGraph &g) { g.remove_vertex(v); }


    // Концепция MutablePropertyGraph
    // =============================================

    /// Добавляет вершину со свойствами vp в граф g.
    /// \return Дескриптор добавленной вершины
    inline SpuUltraGraph::vertex_descriptor add_vertex(SpuUltraGraph::edge_property_type vp, SpuUltraGraph &g) {
        return g.add_vertex(vp.first, vp.second);
    }
    /// Добавляет ребро со свойствами ep между вершинами u и v в граф g.
    /// Если граф запрещает параллельные ребра, то флаг устанавливается в значение false.
    inline std::pair<SpuUltraGraph::edge_descriptor, bool>
    add_edge(SpuUltraGraph::vertex_descriptor from, SpuUltraGraph::vertex_descriptor to, SpuUltraGraph::edge_property_type ep, SpuUltraGraph &g) {
        try {
            return {g.add_edge(ep.first, from, to, ep.second), true};
        } catch (Conflict &e) {
            return {0, false};
        }
    }


    // Концепция VertexListGraph
    // =============================================

    /// Количество вершин в графе g.
    inline SpuUltraGraph::vertices_size_type num_vertices(const SpuUltraGraph &g) { return g.num_vertices(); }
    /// Возвращает диапазон итераторов, обеспечивающий доступ ко всем вершинам в графе g.
    inline std::pair<SpuUltraGraph::vertex_iterator, SpuUltraGraph::vertex_iterator> vertices(const SpuUltraGraph &g) {
        auto vertices = g.vertices();
        return {vertices.begin(), vertices.end()};
    }


    // Концепция EdgeListGraph
    // =============================================

    /// Количество ребер в графе g.
    inline SpuUltraGraph::edges_size_type num_edges(const SpuUltraGraph &g) { return g.num_edges(); }
    /// Возвращает диапазон итераторов, обеспечивающий доступ ко всем ребрам.
    inline std::pair<SpuUltraGraph::edge_iterator, SpuUltraGraph::edge_iterator> edges(const SpuUltraGraph &g) {
        auto edges = g.edges();
        return {edges.begin(), edges.end()};
    }
    /// Возвращает источник для ребра e.
    inline SpuUltraGraph::vertex_descriptor source(SpuUltraGraph::edge_descriptor e, const SpuUltraGraph &g) { return g.source(e); }
    /// Возвращает сток для ребра e.
    inline SpuUltraGraph::vertex_descriptor target(SpuUltraGraph::edge_descriptor e, const SpuUltraGraph &g) { return g.target(e); }


    // Концепция IncidenceGraph
    // =============================================

    /// Возвращает количество исходящих ребер.
    inline SpuUltraGraph::degree_size_type out_degree(SpuUltraGraph::vertex_descriptor v, const SpuUltraGraph &g) { return g.out_degree(v); }
    /// Возвращает диапазон итераторов, обеспечивающий доступ к исходящим ребрам вершины u в графе g.
    inline std::pair<SpuUltraGraph::out_edge_iterator, SpuUltraGraph::out_edge_iterator>
    out_edges(SpuUltraGraph::vertex_descriptor v, const SpuUltraGraph &g) {
        auto edges = g.out_edges(v);
        return {edges.begin(), edges.end()};
    }


    // Концепция BidirectionalGraph
    // =============================================

    /// Возвращает степень вершины.
    inline SpuUltraGraph::degree_size_type degree(SpuUltraGraph::vertex_descriptor v, const SpuUltraGraph &g) { return g.degree(v); }
    /// Возвращает количество входящих ребер.
    inline SpuUltraGraph::degree_size_type in_degree(SpuUltraGraph::vertex_descriptor v, const SpuUltraGraph &g) { return g.in_degree(v); }
    /// Возвращает диапазон итераторов, обеспечивающий доступ к входящим ребрам для вершины v в графе g.
    inline std::pair<SpuUltraGraph::in_edge_iterator, SpuUltraGraph::in_edge_iterator>
    in_edges(SpuUltraGraph::vertex_descriptor v, const SpuUltraGraph &g) {
        auto edges = g.in_edges(v);
        return {edges.begin(), edges.end()};
    }


    // Концепция AdjacencyMatrix
    // =============================================

    /// Возвращает пару, состоящую из флага, указывающего, существует ли ребро между u и v в графе g, и дескриптора ребра
    inline pair<SpuUltraGraph::edge_descriptor, bool>
    edge(SpuUltraGraph::vertex_descriptor u, SpuUltraGraph::vertex_descriptor v, const SpuUltraGraph &g) {
        auto e = g.edge(u, v);
        return {e, bool(e)};
    }


    // Концепция AdjacencyMatrix
    // =============================================

    /// Возвращает диапазон итераторов, обеспечивающий доступ к смежным вершинам для вершины v в графе g.
    inline std::pair<SpuUltraGraph::adjacency_iterator, SpuUltraGraph::adjacency_iterator>
    adjacent_vertices(SpuUltraGraph::vertex_descriptor v, const SpuUltraGraph &g) {
        auto vertices = g.adjacent_vertices(v);
        return {vertices.begin(), vertices.end()};
    }
}


#endif //GRAPH_API_SPUULTRAGRAPHADAPTER_H
