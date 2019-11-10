//
// Created by kiryanenko on 10.05.19.
//

#include "SpuUltraGraph.h"
#include <math.h>
#include <iostream>


using namespace std;


namespace SPU_GRAPH
{

    SpuUltraGraph::SpuUltraGraph(SPU_GRAPH::id_t graph_id, SPU_GRAPH::SpuUltraGraphTraits spu_graph_traits) :
            _graph_id(graph_id),
            _graph_traits(spu_graph_traits),
            _vertex_fields(FieldsLength<SPU_STRUCTURE_ATTRS>()),
            _edge_fields(FieldsLength<SPU_STRUCTURE_ATTRS>())
    {
        if (!_graph_traits.vertex_id_depth || !_graph_traits.edge_id_depth) {
            throw BadRequest("vertex_id_depth and edge_id_depth should be > 0");
        }

        if (_graph_traits.depth_sum() > KEY_DEPTH) {
            throw PayloadTooLarge();
        }

        if (graph_id >= pow(2, _graph_traits.graph_id_depth)) {
            throw PayloadTooLarge();
        }

        FieldsLength<SPU_STRUCTURE_ATTRS> vertex_fields_length({
                                                                       {GRAPH_ID, 0},
                                                                       {INCIDENCE, 1},
                                                                       {VERTEX_ID, 0},
                                                                       {WEIGHT, 0},
                                                                       {EDGE_ID, 0}
                                                               });
        vertex_fields_length[GRAPH_ID] = _graph_traits.graph_id_depth;
        vertex_fields_length[VERTEX_ID] = _graph_traits.vertex_id_depth;
        vertex_fields_length[WEIGHT] = _graph_traits.weight_id_depth;
        vertex_fields_length[EDGE_ID] = _graph_traits.edge_id_depth;
        _vertex_fields = Fields<SPU_STRUCTURE_ATTRS>(vertex_fields_length);

        FieldsLength<SPU_STRUCTURE_ATTRS> edge_fields_length({
                                                                     {GRAPH_ID, 0},
                                                                     {INCIDENCE, 1},
                                                                     {EDGE_ID, 0},
                                                                     {VERTEX_ID, 0}
                                                             });
        edge_fields_length[GRAPH_ID] = _graph_traits.graph_id_depth;
        edge_fields_length[VERTEX_ID] = _graph_traits.vertex_id_depth;
        edge_fields_length[EDGE_ID] = _graph_traits.edge_id_depth;
        _edge_fields = Fields<SPU_STRUCTURE_ATTRS>(edge_fields_length);

        if (!_graph_traits.vertex_struct) {
            _graph_traits.vertex_struct = new Structure<>;
            _should_free_vertex_struct = true;
        }
        if (!_graph_traits.edge_struct) {
            _graph_traits.edge_struct = new Structure<>;
            _should_free_edge_struct = true;
        }
    }

    SpuUltraGraph::~SpuUltraGraph() {
        if (_should_free_vertex_struct) {
            delete _graph_traits.vertex_struct;
        }
        if (_should_free_edge_struct) {
            delete _graph_traits.edge_struct;
        }
    }
}