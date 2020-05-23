//
// Created by a.kiryanenko on 3/26/20.
//

#include "../SpuUltraGraphAdapter.h"
#include "../SpuUltraGraphProperty.h"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
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
void kruskal_test(G &g) {
    typedef typename graph_traits<G>::vertex_descriptor vertex_t;
    typedef typename graph_traits<G>::edge_descriptor edge_t;

    std::vector < edge_t > spanning_tree;
    // Выполняю алгоритм Краскала для построения минимального остовного дерева
    kruskal_minimum_spanning_tree(g, std::back_inserter(spanning_tree));
}


int main()
{
    cout << "SpuUltraGraph performance test" << endl;
    cout << "==========================================" << endl;
    GraphPerformanceTest<SpuUltraGraph> spu_graph_test(kruskal_test, "kruskal_test_SpuUltraGraph.csv");
    spu_graph_test.is_mutable_test = false;
    spu_graph_test.add_edge_func = add_weight_edge;
    spu_graph_test.start();

    cout << "adjacency_list performance test" << endl;
    cout << "==========================================" << endl;
    GraphPerformanceTest<AdjacencyListGraph> adjacency_list_test(kruskal_test, "kruskal_test_adjacency_list.csv");
    adjacency_list_test.is_mutable_test = false;
    adjacency_list_test.add_edge_func = add_weight_edge;
    adjacency_list_test.start();

    cout << "adjacency_matrix performance test" << endl;
    cout << "==========================================" << endl;
    GraphPerformanceTest<AdjacencyMatrixGraph> adjacency_matrix_test(kruskal_test, "kruskal_test_adjacency_matrix.csv");
    adjacency_matrix_test.is_mutable_test = false;
    adjacency_matrix_test.add_edge_func = add_weight_edge;
    adjacency_matrix_test.end_vertices_cnt = 20000;
    adjacency_matrix_test.start();
    return 0;
}