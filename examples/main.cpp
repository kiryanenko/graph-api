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
    traits.vertex_struct = new Structure<>();
    traits.edge_struct = new Structure<>();

    // Инициализация графа с id = 5 и указанием черт
    auto custom_graph = new SpuUltraGraph(5, traits);

    // После того как завершили работу с графом следует освободить память
    delete custom_graph;
    delete traits.vertex_struct;
    delete traits.edge_struct;


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

    return 0;
}
