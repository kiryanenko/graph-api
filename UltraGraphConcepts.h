//
// Created by kiryanenko on 21.05.20.
//

#ifndef GRAPH_API_ULTRAGRAPHCONCEPTS_H
#define GRAPH_API_ULTRAGRAPHCONCEPTS_H

#include "SpuUltraGraph.h"

namespace SPU_GRAPH
{
    // Концепция UltraGraph
    // =============================================

    /// Возвращает диапазон итераторов, обеспечивающий доступ к стокам для ребра e в графе g.
    inline std::pair<SpuUltraGraph::target_iterator, SpuUltraGraph::target_iterator>
    targets(SpuUltraGraph::edge_descriptor e, const SpuUltraGraph &g) {
        auto targets = g.targets(e);
        return {targets.begin(), targets.end()};
    }
    /// Возвращает количество вершин «стоков» для ребра e.
    inline SpuUltraGraph::degree_size_type num_targets(SpuUltraGraph::edge_descriptor e, const SpuUltraGraph &g) {
        return g.num_targets(e);
    }


    // Концепция BidirectionalUltraGraph
    // =============================================

    /// Возвращает диапазон итераторов, обеспечивающий доступ к источникам для ребра e в графе g.
    inline std::pair<SpuUltraGraph::source_iterator, SpuUltraGraph::source_iterator>
    sources(SpuUltraGraph::edge_descriptor e, const SpuUltraGraph &g) {
        auto sources = g.sources(e);
        return {sources.begin(), sources.end()};
    }
    /// Возвращает количество вершин «источников» для ребра e.
    inline SpuUltraGraph::degree_size_type num_sources(SpuUltraGraph::edge_descriptor e, const SpuUltraGraph &g) {
        return g.num_sources(e);
    }


    // Концепция MutableUltraGraph
    // =============================================

    /// Присоединяет как источник вершину v к ребру e
    void connect_source(SpuUltraGraph::edge_descriptor e, SpuUltraGraph::vertex_descriptor v, SpuUltraGraph &g) {
        g.connect_source(e, v);
    }
    /// Присоединяет как сток вершину v к ребру e
    void connect_target(SpuUltraGraph::edge_descriptor e, SpuUltraGraph::vertex_descriptor v, SpuUltraGraph &g) {
        g.connect_target(e, v);
    }
    /// Отсоединят источник v от ребра e
    void disconnect_source(SpuUltraGraph::edge_descriptor e, SpuUltraGraph::vertex_descriptor v, SpuUltraGraph &g) {
        g.disconnect_source(e, v);
    }
    /// Отсоединят сток v от ребра e
    void disconnect_target(SpuUltraGraph::edge_descriptor e, SpuUltraGraph::vertex_descriptor v, SpuUltraGraph &g) {
        g.disconnect_target(e, v);
    }
}


#endif //GRAPH_API_ULTRAGRAPHCONCEPTS_H
