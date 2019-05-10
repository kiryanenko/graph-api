//
// Created by kiryanenko on 10.05.19.
//

#ifndef GRAPH_API_SPUULTRAGRAPH_H
#define GRAPH_API_SPUULTRAGRAPH_H


#include "BaseGraph.h"

#define KEY_DEPTH 64

template <class Value>
class SpuUltraGraph : public BaseGraph<Value> {
    size_t _maxNodesCount;
    size_t _maxEdgesCount;
    weight_t _maxWeight;
//    Structure<string> _edgeStructure; TODO

public:
    explicit SpuUltraGraph(size_t maxNodesCount = 0xFFFF, size_t maxEdgesCount = 0xFFFFFF, weight_t maxWeight = 0xFF);

    Node<Value> createNode(id_t id, Value value) {
        throw NotImplemented();
    }
};


#endif //GRAPH_API_SPUULTRAGRAPH_H
