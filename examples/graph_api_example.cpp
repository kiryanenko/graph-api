//
// Created by a.kiryanenko on 2/22/20.
//

#include <iostream>
#include "../SpuUltraGraph.h"

using namespace std;
using namespace SPU_GRAPH;
using namespace boost;

int main()
{
    // Инициализация графа по умолчанию
    // В этом случае id графа будет равно 0
    SpuUltraGraph graph;

    // Объект черт графа
    SpuUltraGraphTraits traits;
    // Задаю битность. По умолчанию она такая:
    traits.graph_id_depth = 3;      // Кол-во бит под id графа
    traits.vertex_id_depth = 28;    // Кол-во бит под id вершины
    traits.edge_id_depth = 28;      // Кол-во бит под индекс ребра (Индекс ребра НЕ id ребра. id ребра состоит из веса и индекса)
    traits.weight_depth = 4;        // Кол-во бит под вес ребра
    // Указывваю структуры, кот будет использовать наш граф
    GraphStructure vertex_struct, edge_struct;
    traits.vertex_struct = vertex_struct;
    traits.edge_struct = edge_struct;

    // Инициализация графа с id = 5 и указанием черт
    SpuUltraGraph custom_graph = SpuUltraGraph(5, traits);


    // SpuUltraGraph::vertex_descriptor - тип идентификатора вершины графа (unsigned long)
    // SpuUltraGraph::edge_descriptor - тип идентификатора ребра графа (unsigned long)


    // Добавление вершины
    SpuUltraGraph::vertex_descriptor v1 = graph.add_vertex();
    // Добаввление вершины с указанием id
    auto v2 = graph.add_vertex(2);
    // Добаввление вершины с id = 3 и value = 123.123
    auto v3 = graph.add_vertex(3, 123.123);


    // Добавление ребра
    SpuUltraGraph::edge_descriptor e1 = graph.add_edge();
    // Добавление ребра с указанием id
    auto e2 = graph.add_edge(2);
    // К ребру можно прикрепить данные
    auto e3 = graph.add_edge(3, data_t("abc"));
    // Добавление ребра от v1 к v2
    auto e12 = graph.add_edge(v1, v2);

    // Чтобы добавить ребро с весом, прежде необходимо сформировать edge_descriptor.
    // Здесь формируется edge_descriptor у которого вес равен 10 и индекс ребра равен 5.
    // Индекс ребра это не id ребра. Как раз id ребра состоит из веса и индекса.
    SpuUltraGraph::edge_descriptor e4 = graph.get_edge_descriptor(5, 3);
    // Добавление ребра e4 от вершины v2 к v3
    graph.add_edge(e4, v2, v3);

    // Получение веса ребра из edge_descriptor
    cout << "Вес ребра e4 = " << graph.get_weight(e4) << endl;
    // Получение данных вершины. Если вершина не найдена будет исключение
    cout << "Данные вершины v3 = " << (double) graph.get_vertex_value(v3) << endl;
    // Получение данных вершины. Если вершина не найдена будет исключение
    cout << "Данные ребра e3 = " << (const char *) graph.get_edge_value(e3) << endl;

    // Проверка наличия вершины
    cout << "Наличие вершины v1 = " << graph.has_vertex(v1) << endl;
    // Проверка наличия ребра
    cout << "Наличие ребра е1 = " << graph.has_edge(e1) << endl;

    cout << "Кол-во вершин = " << graph.num_vertices() << endl;
    cout << "Кол-во ребер = " << graph.num_edges() << endl;
    cout << "Кол-во исходящих ребер у вершины v1 = " << graph.out_degree(v1) << endl;
    cout << "Кол-во входящих ребер у вершины v1 = " << graph.in_degree(v1) << endl;
    cout << "Кол-во вершин 'источников' у ребра e12 = " << graph.num_sources(e12) << endl;
    cout << "Кол-во вершин 'стоков' у ребра e12 = " << graph.num_targets(e12) << endl;


    // Удаление вершины
    graph.remove_vertex(v1);
    // Удаление всех ребер соединенных с вершиной v2
    graph.clear_vertex(v2);

    // Удаление ребра
    graph.remove_edge(e1);
    // Удаление всех ребер от v2 к v3
    graph.remove_edge(v2, v3);


    v1 = graph.add_vertex(1);
    graph.add_edge(10, v1, v2);
    graph.add_edge(11, v1, v2);
    graph.add_edge(12, v1, v3);
    graph.add_edge(13, v2, v1);
    graph.add_edge(14, v2, v1);
    graph.add_edge(15, v3, v1);

    cout << "Итерация по всем вершинам: ";
    for (auto v : graph.vertices())
        cout << v <<  ' ';
    cout << endl;

    cout << "Итерация по всем ребрам: ";
    for (auto e : graph.edges())
        cout << e <<  ' ';
    cout << endl;

    cout << "Итерация по исходящим ребрам у вершины v1: ";
    for (auto e : graph.out_edges(v1))
        cout << e <<  ' ';
    cout << endl;

    cout << "Итерация по входящим ребрам у вершины v1: ";
    for (auto e : graph.in_edges(v1))
        cout << e <<  ' ';
    cout << endl;

    cout << "Итерация по параллельным ребрам от вершины v1 к v2: ";
    for (auto e : graph.parallel_edges(v1, v2))
        cout << e <<  ' ';
    cout << endl;

    cout << "Итерация по смежным вершинам для v1: ";
    for (auto v : graph.adjacent_vertices(v1))
        cout << v <<  ' ';
    cout << endl;

    return 0;
}
