#include <utility>

//
// Created by kiryanenko on 03.05.19.
//

#include "Node.h"


template<class Value>
vector<Edge> Node<Value>::getEdges(Edge::ORDER orderBy, size_t limit) {
    return _graph->getEdgesAtNode(_id, orderBy, limit);
}

template<class Value>
vector<Edge> Node<Value>::getOutgoingEdges(Edge::ORDER orderBy, size_t limit) {
    return _graph->getOutgoingEdgesAtNode(_id, orderBy, limit);
}

template<class Value>
vector<Edge> Node<Value>::getIngoingEdges(Edge::ORDER orderBy, size_t limit) {
    return _graph->getIngoingEdgesAtNode(_id, orderBy, limit);
}

template<class Value>
Edge Node<Value>::getMinEdge() {
    return _graph->getMinEdgeAtNode(_id);
}

template<class Value>
Edge Node<Value>::getMaxEdge() {
    return _graph->getMaxEdgeAtNode(_id);
}

template<class Value>
Edge Node<Value>::getAnyEdge() {
    return _graph->getAnyEdgeForNode(_id);
}

template<class Value>
Edge Node<Value>::getFirstEdge() {
    return _graph->getFirstEdgeAtNode(_id);
}

template<class Value>
Edge Node<Value>::getLastEdge() {
    return _graph->getLastEdgeAtNode(_id);
}

template<class Value>
vector<Edge> Node<Value>::getMinEdges(Edge::ORDER orderBy, size_t limit) {
    return _graph->getMinEdgesAtNode(_id, orderBy, limit);
}

/// Получить соседние ребра с максимальным весом
/// \param orderBy Сортировка [NOTHING, BY_ID, BY_ID_DESC]
/// \param limit Ограничение по количеству, 0 означает, что ограничения нет
template<class Value>
vector<Edge> Node<Value>::getMaxEdges(Edge::ORDER orderBy, size_t limit) {
    return _graph->getMaxEdgesAtNode(_id, orderBy, limit);
}

template<class Value>
Edge Node<Value>::getAnyOutgoingEdge() {
    return _graph->getAnyOutgoingEdgeAtNode(_id);
}

template<class Value>
Edge Node<Value>::getAnyIngoingAnyEdge() {
    return _graph->getAnyIngoingEdgeAtNode(_id);
}
