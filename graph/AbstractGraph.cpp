//
// Created by kiryanenko on 06.11.19.
//

#include "AbstractGraph.h"

template<class Value>
Node<Value> AbstractGraph<Value>::createNode(Value value) {
    auto id = getFreeNodeId();
    putNode(id, value);
}

template<class Value>
Edge<Value> AbstractGraph<Value>::createEdge() {
    auto id = getFreeEdgeId();
    putEdge(id);
}
