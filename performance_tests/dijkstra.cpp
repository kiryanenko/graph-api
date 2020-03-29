//
// Created by a.kiryanenko on 3/26/20.
//

#include "../SpuUltraGraphAdapter.h"
#include "../SpuUltraGraphProperty.h"
#include <boost/graph/dijkstra_shortest_paths_no_color_map.hpp>
#include "GraphPerformanceTest.h"


using namespace SPU_GRAPH;
using namespace boost;


void dijkstra_test(SpuUltraGraph &g) {
    // Создаю свойство предшественник для вершин
    map<SpuUltraGraph::vertex_descriptor, SpuUltraGraph::vertex_descriptor> vertex_to_predecessor;
    associative_property_map<map<SpuUltraGraph::vertex_descriptor, SpuUltraGraph::vertex_descriptor>> predecessor_property_map(vertex_to_predecessor);
    // Создаю свойство расстояние для вершин
    map<SpuUltraGraph::vertex_descriptor, size_t> vertex_to_distance;
    associative_property_map<map<SpuUltraGraph::vertex_descriptor, size_t>> distance_property_map(vertex_to_distance);

    // Выполняю алгоритм дейкстра для подсчета расстояний от вершины #1 до остальных
    dijkstra_shortest_paths_no_color_map(g, 1, predecessor_map(predecessor_property_map).distance_map(distance_property_map));
}


int main()
{
    GraphPerformanceTest<SpuUltraGraph> spu_graph_test(dijkstra_test, "dijkstra_test_SpuUltraGraph.csv");
    spu_graph_test.is_mutable_test = false;
    spu_graph_test.start();
    return 0;
}