//
// Created by a.kiryanenko on 3/10/20.
//

#include <iostream>
#include "../SpuUltraGraphAdapter.h"
#include "../SpuUltraGraphProperty.h"
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/graphviz.hpp>


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

    graph.add_edge(graph.get_free_edge_descriptor(2), 1, 2);
    graph.add_edge(graph.get_free_edge_descriptor(1), 1, 3);
    graph.add_edge(graph.get_free_edge_descriptor(8), 1, 6);
    graph.add_edge(graph.get_free_edge_descriptor(2), 2, 2);
    graph.add_edge(graph.get_free_edge_descriptor(1), 2, 3);
    graph.add_edge(graph.get_free_edge_descriptor(5), 3, 4);
    graph.add_edge(graph.get_free_edge_descriptor(6), 3, 5);
    graph.add_edge(graph.get_free_edge_descriptor(3), 4, 6);
    graph.add_edge(graph.get_free_edge_descriptor(7), 5, 6);

    write_graphviz(cout, graph);
    return 0;
}
