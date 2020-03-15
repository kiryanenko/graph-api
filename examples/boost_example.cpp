//
// Created by a.kiryanenko on 3/10/20.
//

#include <iostream>
#include "../SpuUltraGraph.h"

using namespace std;
using namespace SPU_GRAPH;
using namespace boost;

int main()
{
    SpuUltraGraph graph;

    graph.add_vertex(1);
    graph.add_vertex(2);
    graph.add_vertex(3);
    graph.add_vertex(4);
    graph.add_vertex(5);
    graph.add_vertex(6);

    graph.add_edge(graph.get_edge_descriptor(1, 2), 1, 2);
    return 0;
}
