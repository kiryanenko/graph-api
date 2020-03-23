//
// Created by a.kiryanenko on 3/10/20.
//

#include <iostream>
#include "../SpuUltraGraphAdapter.h"
#include "../SpuUltraGraphProperty.h"
#include <boost/graph/graphviz.hpp>
#include <boost/graph/dijkstra_shortest_paths_no_color_map.hpp>


using namespace std;
using namespace SPU_GRAPH;
using namespace boost;


// Класс для печати свойств вершины для graphvis
class vertex_property_writer {
    const SpuUltraGraph &_g;
public:
    vertex_property_writer(SpuUltraGraph &g) : _g(g) {}
    void operator()(std::ostream& out, const SpuUltraGraph::vertex_descriptor &v) const {
        out << "[label=\"" << v << "\"]";
    }
};

// Класс для печати свойств ребра для graphvis
class edge_property_writer {
    const SpuUltraGraph &_g;
public:
    edge_property_writer(SpuUltraGraph &g) : _g(g) {}
    void operator()(std::ostream& out, const SpuUltraGraph::edge_descriptor &e) const {
        out << "[label=\"" << get(edge_weight, _g, e) << "\"]";
    }
};


int main()
{
    SpuUltraGraph graph;

    graph.add_vertex(1);
    graph.add_vertex(2);
    graph.add_vertex(3);
    graph.add_vertex(4);
    graph.add_vertex(5);
    graph.add_vertex(10);

    graph.add_edge(graph.get_free_edge_descriptor(2), 1, 2);
    graph.add_edge(graph.get_free_edge_descriptor(1), 1, 3);
    graph.add_edge(graph.get_free_edge_descriptor(8), 1, 10);
    graph.add_edge(graph.get_free_edge_descriptor(1), 2, 2);
    graph.add_edge(graph.get_free_edge_descriptor(1), 2, 3);
    graph.add_edge(graph.get_free_edge_descriptor(2), 2, 4);
    graph.add_edge(graph.get_free_edge_descriptor(5), 3, 4);
    graph.add_edge(graph.get_free_edge_descriptor(6), 3, 5);
    graph.add_edge(graph.get_free_edge_descriptor(3), 4, 10);
    graph.add_edge(graph.get_free_edge_descriptor(7), 5, 10);


    // С помощью GraphViz создадим svg изображение графа
    vertex_property_writer vpw(graph);
    edge_property_writer epw(graph);
    std::ofstream f("graph.dot");
    boost::write_graphviz(f, graph, vpw, epw);
    f.close();
    system("dot graph.dot -Kcirco -Tsvg -o graph.svg");


    // Создаю свойство предшественник для вершин
    map<SpuUltraGraph::vertex_descriptor, SpuUltraGraph::vertex_descriptor> vertex_to_predecessor;
    associative_property_map<map<SpuUltraGraph::vertex_descriptor, SpuUltraGraph::vertex_descriptor>> predecessor_property_map(vertex_to_predecessor);
    // Создаю свойство расстояние для вершин
    map<SpuUltraGraph::vertex_descriptor, size_t> vertex_to_distance;
    associative_property_map<map<SpuUltraGraph::vertex_descriptor, size_t>> distance_property_map(vertex_to_distance);

    // Выполняю алгоритм дейкстра для подсчета расстояний от вершины #1 до остальных
    dijkstra_shortest_paths_no_color_map(graph, 1, predecessor_map(predecessor_property_map).distance_map(distance_property_map));

    std::cout << "Distances and parents:" << std::endl;
    for (auto v: graph.vertices()) {
        cout << "Vertex " << v << " Distance = " << vertex_to_distance[v] << ", parent = " << vertex_to_predecessor[v] + 1 << endl;
    }
    return 0;
}
