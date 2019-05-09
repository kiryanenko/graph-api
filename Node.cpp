#include <utility>

//
// Created by kiryanenko on 03.05.19.
//

#include "Node.h"
#include <stdexcept>

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

template<class Value>
void Node<Value>::setId(id_t newId, bool commit) {
    id = newId;
    if (commit) {
        save();
    }
}


template<class Value>
void Node<Value>::setValue(Value newValue, bool commit) {
    value = newValue;
    if (commit) {
        save();
    }
}


template<class Value>
void Node<Value>::save() {
    if (_commit) {
        Node<Value> newNode = _graph->setNode(_id, id, value);
        _id = id = newNode.id;
        value = newNode.value;
        _commit = true;
    } else {
        Node<Value> newNode = _graph->createNode(id, value);
        _id = id = newNode.id;
        value = newNode.value;
        _commit = true;
    }
}

template<class Value>
void Node<Value>::remove() {
    if (_commit) {
        _graph->removeNode(_id);
        _commit = false;
    }
}

template<class Value>
Edge Node<Value>::connectToNode(id_t nodeId, weight_t weight, bool oriented) {
    if (_commit) {
        return _graph->connectNodes(_id, nodeId, weight, oriented);
    } else {
        throw MethodNotAllowed();
    }
}

template<class Value>
Edge Node<Value>::getMinOutgoingEdge() {
    return _graph->getMinOutgoingEdgeAtNode(_id);
}

template<class Value>
Edge Node<Value>::getMaxOutgoingEdge() {
    return _graph->getMaxOutgoingEdgeAtNode(_id);
}

template<class Value>
Edge Node<Value>::getMinIngoingEdge() {
    return _graph->getMinIngoingEdgeAtNode(_id);
}

template<class Value>
Edge Node<Value>::getMaxIngoingEdge() {
    return _graph->getMaxIngoingEdgeAtNode(_id);
}

template<class Value>
vector<Edge> Node<Value>::getMinOutgoingEdges(Edge::ORDER orderBy, size_t limit) {
    return _graph->getMinOutgoingEdgesAtNode(_id);
}

template<class Value>
vector<Edge> Node<Value>::getMaxOutgoingEdges(Edge::ORDER orderBy, size_t limit) {
    return _graph->getMaxOutgoingEdgesAtNode(_id);
}

template<class Value>
vector<Edge> Node<Value>::getMinIngoingEdges(Edge::ORDER orderBy, size_t limit) {
    return _graph->getMinIngoingEdgesAtNode(_id);
}

template<class Value>
vector<Edge> Node<Value>::getMaxIngoingEdges(Edge::ORDER orderBy, size_t limit) {
    return _graph->getMaxIngoingEdgesAtNode(_id);
}

template<class Value>
Edge Node<Value>::getFirstOutgoingEdge() {
    return _graph->getFirstOutgoingEdgesAtNode(_id);
}

template<class Value>
Edge Node<Value>::getLastOutgoingEdge() {
    return _graph->getLastOutgoingEdgesAtNode(_id);
}

template<class Value>
Edge Node<Value>::getFirstIngoingEdge() {
    return _graph->getFirstIngoingEdgesAtNode(_id);
}

template<class Value>
Edge Node<Value>::getLastIngoingEdge() {
    return _graph->getLastIngoingEdgesAtNode(_id);
}

template<class Value>
size_t Node<Value>::edgesCount() {
    return _graph->edgesCountAtNode(_id);
}

template<class Value>
size_t Node<Value>::outgoingEdgesCount() {
    return _graph->outgoingEdgesCountAtNode(_id);
}

template<class Value>
size_t Node<Value>::ingoingEdgesCount() {
    return _graph->ingoingEdgesCountAtNode(_id);
}
