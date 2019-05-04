//
// Created by kiryanenko on 03.05.19.
//

#ifndef GRAPH_API_NODE_H
#define GRAPH_API_NODE_H

#include <vector>

#include "BaseGraph.h"
#include "Edge.h"

using namespace std;

template <class Value>
class Node {
    BaseGraph *_graph;
    id_t _id = 0;
    Value _value;


public:
    Node(BaseGraph *graph, Value value) : _graph(graph), _value(value) {}

    Node(BaseGraph *graph, id_t id, Value value) : _graph(graph), _id(id), _value(value) {}


    BaseGraph getGraph() { return _graph; }

    int getId() { return _id; }

    Value getValue() { return _value; }

    /// Получить соседние ребра.
    /// \param orderBy Сортировка
    /// \param limit Ограничение по количеству, 0 означает, что ограничения нет
    vector<Edge> getEdges(Edge::ORDER orderBy = Edge::ORDER::NOTHING, size_t limit = 0);

    /// Получить исходящие ребра.
    /// \param orderBy Сортировка
    /// \param limit Ограничение по количеству, 0 означает, что ограничения нет
    vector<Edge> getOutgoingEdges(Edge::ORDER orderBy = Edge::ORDER::NOTHING, size_t limit = 0);

    /// Получить входящие ребра.
    /// \param orderBy Сортировка
    /// \param limit Ограничение по количеству, 0 означает, что ограничения нет
    vector<Edge> getIngoingEdges(Edge::ORDER orderBy = Edge::ORDER::NOTHING, size_t limit = 0);

    /// Получить любое соседнее ребро
    /// \throw EXCEPTIONS::NOT_FOUND В случае отсутствия соседних ребер
    Edge getAnyEdge();
    /// Получить любое исходящие ребро
    /// \throw EXCEPTIONS::NOT_FOUND В случае отсутствия соседних ребер
    Edge getAnyOutgoingEdge();
    /// Получить любое входящие ребро
    /// \throw EXCEPTIONS::NOT_FOUND В случае отсутствия соседних ребер
    Edge getAnyIngoingAnyEdge();


    /// Получить соседнее ребро с минимальным весом
    /// \throw EXCEPTIONS::NOT_FOUND В случае отсутствия соседних ребер
    Edge getMinEdge();
    /// Получить соседние ребро с максимальным весом
    /// \throw EXCEPTIONS::NOT_FOUND В случае отсутствия соседних ребер
    Edge getMaxEdge();

    /// Получить соседние ребра с минимальным весом
    /// \param orderBy Сортировка [NOTHING, BY_ID, BY_ID_DESC]
    /// \param limit Ограничение по количеству, 0 означает, что ограничения нет
    vector<Edge> getMinEdges(Edge::ORDER orderBy = Edge::ORDER::NOTHING, size_t limit = 0);
    /// Получить соседние ребра с максимальным весом
    /// \param orderBy Сортировка [NOTHING, BY_ID, BY_ID_DESC]
    /// \param limit Ограничение по количеству, 0 означает, что ограничения нет
    vector<Edge> getMaxEdges(Edge::ORDER orderBy = Edge::ORDER::NOTHING, size_t limit = 0);

    /// Получить соседнее ребро с минимальным идентификатором
    /// \throw EXCEPTIONS::NOT_FOUND В случае отсутствия соседних ребер
    Edge getFirstEdge();
    /// Получить соседнее ребро с максимальным идентификатором
    /// \throw EXCEPTIONS::NOT_FOUND В случае отсутствия соседних ребер
    Edge getLastEdge();


};


#endif //GRAPH_API_NODE_H
