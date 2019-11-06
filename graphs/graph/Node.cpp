//
// Created by kiryanenko on 06.11.19.
//

#include "Node.h"

template<class Value>
vector<Edge<Value>> Node<Value>::getEdges(ORDER_BY orderBy, size_t limit) {
    return _graph->getEdgesAtNode(_id, orderBy, limit);
}

template<class Value>
Edge<Value> Node<Value>::getAnyEdge() {
    return _graph->getAnyEdgeForNode(_id);
}

template<class Value>
Edge<Value> Node<Value>::getFirstEdge() {
    return _graph->getFirstEdgeAtNode(_id);
}

template<class Value>
Edge<Value> Node<Value>::getLastEdge() {
    return _graph->getLastEdgeAtNode(_id);
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
    } else {
        Node<Value> newNode = _graph->createNode(id, value);
        _id = id = newNode.id;
        value = newNode.value;
    }
    _commit = true;
}

template<class Value>
void Node<Value>::remove() {
    if (_commit) {
        _graph->removeNode(_id);
        _commit = false;
    }
}

template<class Value>
Edge<Value> Node<Value>::connectWithNode(id_t nodeId, weight_t weight, bool oriented) {
    if (_commit) {
        return _graph->connectNodes(_id, nodeId, weight, oriented);
    } else {
        throw MethodNotAllowed();
    }
}



template<class Value>
size_t Node<Value>::edgesCount() {
    return _graph->edgesCountForNode(_id);
}

