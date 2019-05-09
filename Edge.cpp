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
