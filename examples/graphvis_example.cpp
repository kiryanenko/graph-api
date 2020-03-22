//
// Created by a.kiryanenko on 3/10/20.
//

#include <iostream>
#include "../SpuUltraGraphAdapter.h"
#include "../SpuUltraGraphProperty.h"
#include <boost/graph/graphviz.hpp>


using namespace std;
using namespace SPU_GRAPH;
using namespace boost;


// Класс для печати свойств вершины для graphvis
class vertex_property_writer {
    const SpuUltraGraph &_g;
public:
    vertex_property_writer(SpuUltraGraph &g) : _g(g) {}
    void operator()(std::ostream& out, const SpuUltraGraph::vertex_descriptor &v) const {
        out << "[label=\"#" << get(vertex_index, _g, v) << "\"]";
    }
};

// Класс для печати свойств ребра для graphvis
class edge_property_writer {
    const SpuUltraGraph &_g;
public:
    edge_property_writer(SpuUltraGraph &g) : _g(g) {}
    void operator()(std::ostream& out, const SpuUltraGraph::edge_descriptor &e) const {
        auto weight = get(edge_weight, _g, e);
        out << "[label=\"#" << get(edge_index, _g, e) << "\",xlabel=\"W" << weight << "\",weight=" << weight << "]";
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
    graph.add_vertex(6);

    graph.add_edge(graph.get_free_edge_descriptor(2), 1, 2);
    graph.add_edge(graph.get_free_edge_descriptor(1), 1, 3);
    graph.add_edge(graph.get_free_edge_descriptor(8), 1, 6);
    graph.add_edge(graph.get_free_edge_descriptor(1), 2, 2);
    graph.add_edge(graph.get_free_edge_descriptor(1), 2, 3);
    graph.add_edge(graph.get_free_edge_descriptor(2), 2, 4);
    graph.add_edge(graph.get_free_edge_descriptor(5), 3, 4);
    graph.add_edge(graph.get_free_edge_descriptor(6), 3, 5);
    graph.add_edge(graph.get_free_edge_descriptor(3), 4, 6);
    graph.add_edge(graph.get_free_edge_descriptor(7), 5, 6);


    // Распечатать в консоль граф в формате graphviz
    vertex_property_writer vpw(graph);
    edge_property_writer epw(graph);
    write_graphviz(cout, graph, vpw, epw);
    // С помощью GraphViz создадим svg изображение графа
    std::ofstream f("graph.dot");
    boost::write_graphviz(f, graph, vpw, epw);
    f.close();
    system("dot graph.dot -Kcirco -Tsvg -o graph.svg");
    return 0;
}
