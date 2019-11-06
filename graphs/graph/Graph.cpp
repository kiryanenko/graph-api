//
// Created by kiryanenko on 06.11.19.
//

#include "Graph.h"
#include "../../Node.h"

template<class Value>
Graph<Value>::Graph() {
    graph_id = getFreeGraphId();
}

template<class Value>
Node<Value>& Graph<Value>::createNode(Value value) {
    auto id = getFreeNodeId();
    return insertNode(id, value);
}

template<class Value>
Edge<Value>& Graph<Value>::createEdge() {
    auto id = getFreeEdgeId();
    return insertEdge(id);
}

template<class Value>
Edge<Value> &Graph<Value>::getFirstEdgeForNode(id_t nodeId) {
    auto edges = getEdgesForNode(nodeId, ORDER_BY::ID, 1);
    if (edges.size() > 0) {
        return edges[0];
    } else {
        throw NotFound();
    }
}

template<class Value>
Edge<Value> &Graph<Value>::getLastEdgeForNode(id_t nodeId) {
    auto edges = getEdgesForNode(nodeId, ORDER_BY::ID_DESC, 1);
    if (edges.size() > 0) {
        return edges[0];
    } else {
        throw NotFound();
    }
}

template<class Value>
Edge<Value> &Graph<Value>::getAnyEdgeForNode(id_t nodeId) {
    auto edges = getEdgesForNode(nodeId, NOTHING, 1);
    if (edges.size() > 0) {
        return edges[0];
    } else {
        throw NotFound();
    }
}


