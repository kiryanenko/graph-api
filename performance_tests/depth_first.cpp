//
// Created by a.kiryanenko on 3/26/20.
//

#include "../SpuUltraGraphAdapter.h"
#include "../SpuUltraGraphProperty.h"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/depth_first_search.hpp>
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
void depth_first_test(G &g) {
    // Выполняю алгоритм поиска в глубину
    depth_first_search(g, visitor(default_dfs_visitor()));
}


int main()
{
    cout << "SpuUltraGraph performance test" << endl;
    cout << "==========================================" << endl;
    GraphPerformanceTest<SpuUltraGraph> spu_graph_test(depth_first_test, "kruskal_test_SpuUltraGraph.csv");
    spu_graph_test.is_mutable_test = false;
    spu_graph_test.start();

    cout << "adjacency_list performance test" << endl;
    cout << "==========================================" << endl;
    GraphPerformanceTest<AdjacencyListGraph> adjacency_list_test(depth_first_test, "kruskal_test_adjacency_list.csv");
    adjacency_list_test.is_mutable_test = false;
    adjacency_list_test.start();

    cout << "adjacency_matrix performance test" << endl;
    cout << "==========================================" << endl;
    GraphPerformanceTest<AdjacencyMatrixGraph> adjacency_matrix_test(depth_first_test, "kruskal_test_adjacency_matrix.csv");
    adjacency_matrix_test.is_mutable_test = false;
    adjacency_matrix_test.end_vertices_cnt = 20000;
    adjacency_matrix_test.start();
    return 0;
}