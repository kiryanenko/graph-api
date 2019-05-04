//
// Created by kiryanenko on 03.05.19.
//

#ifndef GRAPH_API_NODE_H
#define GRAPH_API_NODE_H

#include <vector>

#include "BaseGraph.h"
#include "Edge.h"

using namespace std;

template <class Value>
class Node {
    BaseGraph<Value> *_graph;
    /// Идентификатор узла, значение соответствует значению в БД
    id_t _id = 0;
    /// Узел соответсвует узлу хранящиемуся в БД
    bool _commit = false;


public:
    /// Идентификатор узла. После изменения нужно вызвать метод save() для записи в БД.
    id_t id = 0;
    /// Значение узла. После изменения нужно вызвать метод save() для записи в БД.
    Value value;


    Node(BaseGraph<Value> *graph, Value value) : _graph(graph), value(value) {}
    /// Конструктор узла
    /// \param graph Указатель на граф
    /// \param id Идентификатор узла
    /// \param value Значение узла
    /// \param commit Узел соответствует узлу находящимуся в БД. Не рекомендуется выставлять в true.
    Node(BaseGraph<Value> *graph, id_t id, Value value, bool commit=false) :
        _graph(graph), _id(id), id(id), value(value), _commit(commit) {}


    BaseGraph getGraph() { return _graph; }
    int getId() { return id; }
    Value getValue() { return value; }

    /// Изменить id узла
    /// \param newId Новый идентификтор узла
    /// \param commit Записать измения в БД
    void setId(id_t newId, bool commit=false);
    /// Изменить value узла
    /// \param newId Новое значение
    /// \param commit Записать измения в БД
    void setValue(id_t newId, bool commit=false);

    /// Сохранить измения в БД
    void save();
    /// Удалить узел
    void remove();

    /// Получить соседние ребра.
    /// \param orderBy Сортировка
    /// \param limit Ограничение по количеству, 0 означает, что ограничения нет
    vector<Edge> getEdges(Edge::ORDER orderBy = Edge::ORDER::NOTHING, size_t limit = 0);
    /// Получить исходящие ребра.
    /// \param orderBy Сортировка
    /// \param limit Ограничение по количеству, 0 означает, что ограничения нет
    vector<Edge> getOutgoingEdges(Edge::ORDER orderBy = Edge::ORDER::NOTHING, size_t limit = 0);
    /// Получить входящие ребра.
    /// \param orderBy Сортировка
    /// \param limit Ограничение по количеству, 0 означает, что ограничения нет
    vector<Edge> getIngoingEdges(Edge::ORDER orderBy = Edge::ORDER::NOTHING, size_t limit = 0);

    /// Получить любое соседнее ребро
    /// \throw EXCEPTIONS::NOT_FOUND В случае отсутствия соседних ребер
    Edge getAnyEdge();
    /// Получить любое исходящие ребро
    /// \throw EXCEPTIONS::NOT_FOUND В случае отсутствия исходящих ребер
    Edge getAnyOutgoingEdge();
    /// Получить любое входящие ребро
    /// \throw EXCEPTIONS::NOT_FOUND В случае отсутствия входящих ребер
    Edge getAnyIngoingAnyEdge();


    /// Получить соседнее ребро с минимальным весом
    /// \throw EXCEPTIONS::NOT_FOUND В случае отсутствия соседних ребер
    Edge getMinEdge();
    /// Получить соседние ребро с максимальным весом
    /// \throw EXCEPTIONS::NOT_FOUND В случае отсутствия соседних ребер
    Edge getMaxEdge();
    /// Получить исходящее ребро с минимальным весом
    /// \throw EXCEPTIONS::NOT_FOUND В случае отсутствия исходящих ребер
    Edge getMinOutgoingEdge();
    /// Получить исходящее ребро с максимальным весом
    /// \throw EXCEPTIONS::NOT_FOUND В случае отсутствия исходящих ребер
    Edge getMaxOutgoingEdge();
    /// Получить входящих ребро с минимальным весом
    /// \throw EXCEPTIONS::NOT_FOUND В случае отсутствия входящих ребер
    Edge getMinIngoingEdge();
    /// Получить соседние ребро с максимальным весом
    /// \throw EXCEPTIONS::NOT_FOUND В случае отсутствия входящих ребер
    Edge getMaxIngoingEdge();

    /// Получить соседние ребра с минимальным весом
    /// \param orderBy Сортировка [NOTHING, BY_ID, BY_ID_DESC]
    /// \param limit Ограничение по количеству, 0 означает, что ограничения нет
    vector<Edge> getMinEdges(Edge::ORDER orderBy = Edge::ORDER::NOTHING, size_t limit = 0);
    /// Получить соседние ребра с максимальным весом
    /// \param orderBy Сортировка [NOTHING, BY_ID, BY_ID_DESC]
    /// \param limit Ограничение по количеству, 0 означает, что ограничения нет
    vector<Edge> getMaxEdges(Edge::ORDER orderBy = Edge::ORDER::NOTHING, size_t limit = 0);
    /// Получить исходящие ребра с минимальным весом
    /// \param orderBy Сортировка [NOTHING, BY_ID, BY_ID_DESC]
    /// \param limit Ограничение по количеству, 0 означает, что ограничения нет
    vector<Edge> getMinOutgoingEdges(Edge::ORDER orderBy = Edge::ORDER::NOTHING, size_t limit = 0);
    /// Получить входящие ребра с максимальным весом
    /// \param orderBy Сортировка [NOTHING, BY_ID, BY_ID_DESC]
    /// \param limit Ограничение по количеству, 0 означает, что ограничения нет
    vector<Edge> getMaxOutgoingEdges(Edge::ORDER orderBy = Edge::ORDER::NOTHING, size_t limit = 0);
    /// Получить входящих ребра с минимальным весом
    /// \param orderBy Сортировка [NOTHING, BY_ID, BY_ID_DESC]
    /// \param limit Ограничение по количеству, 0 означает, что ограничения нет
    vector<Edge> getMinIngoingEdges(Edge::ORDER orderBy = Edge::ORDER::NOTHING, size_t limit = 0);
    /// Получить входящие ребра с максимальным весом
    /// \param orderBy Сортировка [NOTHING, BY_ID, BY_ID_DESC]
    /// \param limit Ограничение по количеству, 0 означает, что ограничения нет
    vector<Edge> getMaxIngoingEdges(Edge::ORDER orderBy = Edge::ORDER::NOTHING, size_t limit = 0);

    /// Получить соседнее ребро с минимальным идентификатором
    /// \throw EXCEPTIONS::NOT_FOUND В случае отсутствия соседних ребер
    Edge getFirstEdge();
    /// Получить соседнее ребро с максимальным идентификатором
    /// \throw EXCEPTIONS::NOT_FOUND В случае отсутствия соседних ребер
    Edge getLastEdge();
    /// Получить исходящих ребро с минимальным идентификатором
    /// \throw EXCEPTIONS::NOT_FOUND В случае отсутствия исходящих ребер
    Edge getFirstOutgoingEdge();
    /// Получить входящих ребро с максимальным идентификатором
    /// \throw EXCEPTIONS::NOT_FOUND В случае отсутствия входящих ребер
    Edge getLastOutgoingEdge();
    /// Получить входящих ребро с минимальным идентификатором
    /// \throw EXCEPTIONS::NOT_FOUND В случае отсутствия входящих ребер
    Edge getFirstIngoingEdge();
    /// Получить входящих ребро с максимальным идентификатором
    /// \throw EXCEPTIONS::NOT_FOUND В случае отсутствия входящих ребер
    Edge getLastIngoingEdge();

    /// Количество соседних ребер
    size_t edgesCount();
    /// Количество исходящих ребер
    size_t outgoingEdgesCount();
    /// Количество входящих ребер
    size_t ingoingEdgesCount();
};


#endif //GRAPH_API_NODE_H
