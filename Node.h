//
// Created by kiryanenko on 03.05.19.
//

#ifndef GRAPH_API_NODE_H
#define GRAPH_API_NODE_H

#include <vector>

#include "BaseGraph.h"
#include "Edge.h"

using namespace std;


template <class Value> class BaseGraph;
template <class Value> class Edge;


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


    BaseGraph<Value>* getGraph() const { return _graph; }
    int getId() const { return id; }
    Value getValue() const { return value; }

    /// Изменить id узла
    /// \param newId Новый идентификтор узла
    /// \param commit Записать измения в БД
    void setId(id_t newId, bool commit=false);
    /// Изменить value узла
    /// \param newId Новое значение
    /// \param commit Записать измения в БД
    void setValue(Value newValue, bool commit=false);

    /// Сохранить измения в БД
    void save();
    /// Удалить узел
    void remove();

    /// Создать ребро от текущего узла к узлу nodeId
    /// \param nodeId Идентификатор узла
    /// \param weight Вес ребра
    /// \param oriented Ориентированное ребро
    /// \throw MethodNotAllowed Если узел не связан с БД. Вероятно нужно перед выполнить save().
    /// \return Созданное ребро
    Edge<Value> connectToNode(id_t nodeId, weight_t weight, bool oriented=true);

    /// Получить соседние ребра.
    /// \param orderBy Сортировка
    /// \param limit Ограничение по количеству, 0 означает, что ограничения нет
    vector<Edge<Value>> getEdges(ORDER_BY orderBy = ORDER_BY::NOTHING, size_t limit = 0);
    /// Получить исходящие ребра.
    /// \param orderBy Сортировка
    /// \param limit Ограничение по количеству, 0 означает, что ограничения нет
    vector<Edge<Value>> getOutgoingEdges(ORDER_BY orderBy = ORDER_BY::NOTHING, size_t limit = 0);
    /// Получить входящие ребра.
    /// \param orderBy Сортировка
    /// \param limit Ограничение по количеству, 0 означает, что ограничения нет
    vector<Edge<Value>> getIngoingEdges(ORDER_BY orderBy = ORDER_BY::NOTHING, size_t limit = 0);

    /// Получить любое соседнее ребро
    /// \throw NotFound В случае отсутствия соседних ребер
    Edge<Value> getAnyEdge();
    /// Получить любое исходящие ребро
    /// \throw NotFound В случае отсутствия исходящих ребер
    Edge<Value> getAnyOutgoingEdge();
    /// Получить любое входящие ребро
    /// \throw NotFound В случае отсутствия входящих ребер
    Edge<Value> getAnyIngoingAnyEdge();


    /// Получить соседнее ребро с минимальным весом
    /// \throw NotFound В случае отсутствия соседних ребер
    Edge<Value> getMinEdge();
    /// Получить соседние ребро с максимальным весом
    /// \throw NotFound В случае отсутствия соседних ребер
    Edge<Value> getMaxEdge();
    /// Получить исходящее ребро с минимальным весом
    /// \throw NotFound В случае отсутствия исходящих ребер
    Edge<Value> getMinOutgoingEdge();
    /// Получить исходящее ребро с максимальным весом
    /// \throw NotFound В случае отсутствия исходящих ребер
    Edge<Value> getMaxOutgoingEdge();
    /// Получить входящих ребро с минимальным весом
    /// \throw NotFound В случае отсутствия входящих ребер
    Edge<Value> getMinIngoingEdge();
    /// Получить соседние ребро с максимальным весом
    /// \throw NotFound В случае отсутствия входящих ребер
    Edge<Value> getMaxIngoingEdge();

    /// Получить соседние ребра с минимальным весом
    /// \param orderBy Сортировка [NOTHING, BY_ID, BY_ID_DESC]
    /// \param limit Ограничение по количеству, 0 означает, что ограничения нет
    vector<Edge<Value>> getMinEdges(ORDER_BY orderBy = ORDER_BY::NOTHING, size_t limit = 0);
    /// Получить соседние ребра с максимальным весом
    /// \param orderBy Сортировка [NOTHING, BY_ID, BY_ID_DESC]
    /// \param limit Ограничение по количеству, 0 означает, что ограничения нет
    vector<Edge<Value>> getMaxEdges(ORDER_BY orderBy = ORDER_BY::NOTHING, size_t limit = 0);
    /// Получить исходящие ребра с минимальным весом
    /// \param orderBy Сортировка [NOTHING, BY_ID, BY_ID_DESC]
    /// \param limit Ограничение по количеству, 0 означает, что ограничения нет
    vector<Edge<Value>> getMinOutgoingEdges(ORDER_BY orderBy = ORDER_BY::NOTHING, size_t limit = 0);
    /// Получить входящие ребра с максимальным весом
    /// \param orderBy Сортировка [NOTHING, BY_ID, BY_ID_DESC]
    /// \param limit Ограничение по количеству, 0 означает, что ограничения нет
    vector<Edge<Value>> getMaxOutgoingEdges(ORDER_BY orderBy = ORDER_BY::NOTHING, size_t limit = 0);
    /// Получить входящих ребра с минимальным весом
    /// \param orderBy Сортировка [NOTHING, BY_ID, BY_ID_DESC]
    /// \param limit Ограничение по количеству, 0 означает, что ограничения нет
    vector<Edge<Value>> getMinIngoingEdges(ORDER_BY orderBy = ORDER_BY::NOTHING, size_t limit = 0);
    /// Получить входящие ребра с максимальным весом
    /// \param orderBy Сортировка [NOTHING, BY_ID, BY_ID_DESC]
    /// \param limit Ограничение по количеству, 0 означает, что ограничения нет
    vector<Edge<Value>> getMaxIngoingEdges(ORDER_BY orderBy = ORDER_BY::NOTHING, size_t limit = 0);

    /// Получить соседнее ребро с минимальным идентификатором
    /// \throw NotFound В случае отсутствия соседних ребер
    Edge<Value> getFirstEdge();
    /// Получить соседнее ребро с максимальным идентификатором
    /// \throw NotFound В случае отсутствия соседних ребер
    Edge<Value> getLastEdge();
    /// Получить исходящих ребро с минимальным идентификатором
    /// \throw NotFound В случае отсутствия исходящих ребер
    Edge<Value> getFirstOutgoingEdge();
    /// Получить входящих ребро с максимальным идентификатором
    /// \throw NotFound В случае отсутствия входящих ребер
    Edge<Value> getLastOutgoingEdge();
    /// Получить входящих ребро с минимальным идентификатором
    /// \throw NotFound В случае отсутствия входящих ребер
    Edge<Value> getFirstIngoingEdge();
    /// Получить входящих ребро с максимальным идентификатором
    /// \throw NotFound В случае отсутствия входящих ребер
    Edge<Value> getLastIngoingEdge();

    /// Количество соседних ребер
    size_t edgesCount();
    /// Количество исходящих ребер
    size_t outgoingEdgesCount();
    /// Количество входящих ребер
    size_t ingoingEdgesCount();
};


#endif //GRAPH_API_NODE_H
