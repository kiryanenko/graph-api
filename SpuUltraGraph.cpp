//
// Created by kiryanenko on 10.05.19.
//

#include "SpuUltraGraph.h"
#include <math.h>

template<class Value>
SpuUltraGraph<Value>::SpuUltraGraph(size_t maxNodesCount, size_t maxEdgesCount, weight_t maxWeight) :
    _maxNodesCount(maxNodesCount), _maxEdgesCount(maxEdgesCount), _maxWeight(maxWeight) {
    if (not maxNodesCount or not maxEdgesCount or not maxWeight) {
        throw BadRequest("maxNodesCount and maxEdgesCount and maxWeight should be > 0");
    }

    size_t nodeIdDepth = ceil(log2(maxNodesCount));
    size_t edgeIdDepth = ceil(log2(maxEdgesCount));
    size_t weightDepth = ceil(log2(maxWeight));

    if (nodeIdDepth + edgeIdDepth + weightDepth > KEY_DEPTH) {
        throw PayloadTooLarge();
    }
}
