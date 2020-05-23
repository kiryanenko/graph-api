//
// Created by a.kiryanenko on 3/26/20.
//

#include "../SpuUltraGraphAdapter.h"
#include "../SpuUltraGraphProperty.h"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths_no_color_map.hpp>
#include "GraphPerformanceTest.h"


using namespace SPU_GRAPH;
using namespace boost;


typedef boost::adjacency_list <
        boost::vecS, // как хранить вершины - в векторе
        boost::vecS, // как хранить ребра из каждой вершины - в векторе
        boost::directedS,
        no_property,
        property < edge_weight_t, int >
> AdjacencyListGraph;



template <class G>
pair<typename graph_traits<G>::edge_descriptor, bool>
add_weight_edge(typename graph_traits<G>::vertex_descriptor u, typename graph_traits<G>::vertex_descriptor v, G& g) {
    auto weight = rand() % 16;
    return add_edge(u, v, weight, g);
}

template <>
pair<typename graph_traits<SpuUltraGraph>::edge_descriptor, bool>
add_weight_edge(typename graph_traits<SpuUltraGraph>::vertex_descriptor u, typename graph_traits<SpuUltraGraph>::vertex_descriptor v, SpuUltraGraph& g) {
    auto weight = rand() % 16;
    return {g.add_edge(g.get_free_edge_descriptor(weight), u, v), true};
}


template <class G>
void dijkstra_test(G &g) {
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
    cout << "SpuUltraGraph performance test" << endl;
    cout << "==========================================" << endl;
    GraphPerformanceTest<SpuUltraGraph> spu_graph_test(dijkstra_test, "dijkstra_test_SpuUltraGraph.csv");
    spu_graph_test.is_mutable_test = false;
    spu_graph_test.add_edge_func = add_weight_edge;
    spu_graph_test.start();

    cout << "adjacency_list performance test" << endl;
    cout << "==========================================" << endl;
    GraphPerformanceTest<AdjacencyListGraph> adjacency_list_test(dijkstra_test, "dijkstra_test_adjacency_list.csv");
    adjacency_list_test.is_mutable_test = false;
    adjacency_list_test.add_edge_func = add_weight_edge;
    adjacency_list_test.start();

    cout << "adjacency_matrix performance test" << endl;
    cout << "==========================================" << endl;
    GraphPerformanceTest<AdjacencyMatrixGraph> adjacency_matrix_test(dijkstra_test, "dijkstra_test_adjacency_matrix.csv");
    adjacency_matrix_test.is_mutable_test = false;
    adjacency_matrix_test.add_edge_func = add_weight_edge;
    adjacency_matrix_test.start();
    return 0;
}