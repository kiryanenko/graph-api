//
// Created by kiryanenko on 10.05.19.
//

#include "SpuUltraGraph.h"
#include <math.h>

SPU_GRAPH::SpuUltraGraph::SpuUltraGraph(id_t graph_id, size_t graph_id_depth, size_t node_id_depth, size_t edge_id_depth,
                                        size_t weight_depth) {
    if (!node_id_depth || !edge_id_depth) {
        throw BadRequest("graph_id_depth and edge_id_depth should be > 0");
    }

    if (graph_id_depth + node_id_depth + edge_id_depth + weight_depth > KEY_DEPTH) {
        throw PayloadTooLarge();
    }

    _graph_id_depth = graph_id_depth;
    _vertex_id_depth = node_id_depth;
    _edge_id_depth = edge_id_depth;
    _weight_id_depth = weight_depth;

    if (graph_id >= pow(2, graph_id_depth)) {
        throw PayloadTooLarge();
    }
}
