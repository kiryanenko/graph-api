//
// Created by kiryanenko on 03.05.19.
//

#include "Edge.h"

template<class Value>
void Edge<Value>::setId(id_t newId, bool commit) {
    id = newId;
    if (commit) {
        save();
    }
}

template<class Value>
void Edge<Value>::setValue(weight_t newWeight, bool commit) {
    weight = newWeight;
    if (commit) {
        save();
    }
}

template<class Value>
void Edge<Value>::save() {
    if (_commit) {
        Edge newEdge = _graph->setEdge(_id, id, weight);
        _id = id = newEdge.id;
        weight = newEdge.weight;
    } else {
        Edge newEdge = _graph->createEdge(id, weight);
        _id = id = newEdge.id;
        weight = newEdge.weight;
    }
    _commit = true;
}

template<class Value>
void Edge<Value>::remove() {
    if (_commit) {
        _graph->removeEdge(_id);
        _commit = false;
    }
}


template<class Value>
void Edge<Value>::addNode(id_t nodeId) {
    if (_commit) {
        _graph->addEdgeToNode(nodeId, _id);
    } else {
        throw MethodNotAllowed();
    }
}

template<class Value>
void Edge<Value>::addSrcNode(id_t nodeId) {
    if (_commit) {
        _graph->addOutgoingEdgeToNode(nodeId, _id);
    } else {
        throw MethodNotAllowed();
    }
}

template<class Value>
void Edge<Value>::addDstNode(id_t nodeId) {
    if (_commit) {
        _graph->addIngoingEdgeToNode(nodeId, _id);
    } else {
        throw MethodNotAllowed();
    }
}

template<class Value>
vector<Node<Value>> Edge<Value>::getNodes(ORDER_BY order, size_t limit, size_t offset) {
    return _graph->getNodesForEdge(_id,  order, limit, offset);
}

template<class Value>
Node<Value> Edge<Value>::getSrcNode() {
    return _graph->getSrcNodeForEdge(_id);
}

template<class Value>
vector<Node<Value>> Edge<Value>::getSrcNodes(ORDER_BY order, size_t limit, size_t offset) {
    return _graph->getSrcNodesForEdge(_id,  order, limit, offset);
}

template<class Value>
Node<Value> Edge<Value>::getDstNode() {
    return _graph->getDstNodeForEdge(_id);
}

template<class Value>
vector<Node<Value>> Edge<Value>::getDstNodes(ORDER_BY order, size_t limit, size_t offset) {
    return _graph->getDstNodesForEdge(_id,  order, limit, offset);
}

