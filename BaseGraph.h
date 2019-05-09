//
// Created by kiryanenko on 03.05.19.
//

#ifndef GRAPH_API_BASEGRAPH_H
#define GRAPH_API_BASEGRAPH_H

#include <vector>
#include "exceptions.h"
#include "types.h"
#include "Edge.h"
#include "Node.h"


using namespace std;


template <class Value> class Edge;
template <class Value> class Node;


template <class Value>
class BaseGraph {
public:
    virtual ~BaseGraph() {}

    ///////////////  Базовые операции с узлом  //////////////////////////

    /// Создать узел
    /// \param id идентификатор. Если id = 0 то узлу присвоится свободный идентификатор
    /// \param value значение
    /// \throw Conflict Если есть узел с таким же id
    /// \return Созданный узел
    virtual Node<Value> createNode(id_t id, Value value) {
        throw NotImplemented();
    }

    /// Создать узел. Узлу будет назначен свободный идентификатор
    /// \param value значение
    /// \return Созданный узел
    virtual Node <Value> createNode(Value value) {
        throw NotImplemented();
    }

    /// Изменить узел
    /// \param nodeId Идентификатор узла в БД
    /// \param newId Новый идентификатор
    /// \param newValue Новое значение
    /// \throw NotFound Если узел не найден
    /// \return Узел соответствующий БД
    virtual Node <Value> setNode(id_t nodeId, id_t newId, Value newValue) {
        throw NotImplemented();
    }

    /// Изменить узел
    /// \param nodeId Идентификатор узла в БД
    /// \param newId Новый идентификатор
    /// \param newValue Новое значение
    /// \throw NotFound Если узел не найден
    /// \return Узел соответствующий БД
    Node <Value> updateNode(id_t nodeId, id_t newId, Value newValue) {
        setNode(nodeId, newId, newValue);
    }

    /// Обновить узел, если его нет то добавить
    /// \param nodeId Идентификатор узла
    /// \param value Новое значение
    /// \return Узел соответствующий БД
    Node <Value> updateOrCreateNode(id_t nodeId, Value value) {
        throw NotImplemented();
    }

    /// Обновить узел, если его нет то добавить
    /// \param nodeId Идентификатор узла
    void removeNode(id_t nodeId) {
        throw NotImplemented();
    }


    ///////////////  Базовые операции с ребром  //////////////////////////

    /// Создать ребро
    /// \param id идентификатор. Если id = 0 то узлу присвоится свободный идентификатор
    /// \param value значение
    /// \throw Conflict Если есть узел с таким же id
    /// \return Созданный ребро
    virtual Edge<Value> createEdge(id_t id, weight_t weight) {
        throw NotImplemented();
    }

    /// Создать ребро. Ребру будет назначен свободный идентификатор
    /// \param weight вес
    /// \return Созданный ребро
    virtual Edge<Value> createEdge(weight_t weight) {
        throw NotImplemented();
    }

    /// Изменить ребро
    /// \param nodeId Идентификатор узла в БД
    /// \param newId Новый идентификатор
    /// \param newWeight Новый вес
    /// \throw NotFound Если узел не найден
    /// \return Узел соответствующий БД
    virtual Edge<Value> setEdge(id_t edgeId, id_t newId, weight_t newWeight) {
        throw NotImplemented();
    }

    /// Изменить ребро
    /// \param edgeId Идентификатор ребра в БД
    /// \param newId Новый идентификатор
    /// \param newWeight Новое значение
    /// \throw NotFound Если узел не найден
    /// \return Узел соответствующий БД
    Edge<Value> updateEdge(id_t edgeId, id_t newId, weight_t newWeight) {
        setEdge(edgeId, newId, newWeight);
    }

    /// Обновить ребро, если его нет то добавить
    /// \param nodeId Идентификатор узла
    /// \param weight Новое значение
    /// \return Ребро соответствующее БД
    Edge<Value> updateOrCreateEdge(id_t nodeId, weight_t weight) {
        throw NotImplemented();
    }

    /// Обновить ребро, если его нет то добавить
    /// \param edgeId Идентификатор
    void removeEdge(id_t edgeId) {
        throw NotImplemented();
    }



    /// Создать ребро от узла srcNodeId к узлу dstNodeId
    /// \param srcNodeId Идентификатор узла источника
    /// \param dstNodeId Идентификатор узла цели
    /// \param weight Вес ребра
    /// \param oriented Ориентированное ребро
    /// \return Созданное ребро
    Edge<Value> connectNodes(id_t srcNodeId, id_t dstNodeId, weight_t weight, bool oriented) {
        throw NotImplemented();
    }

    /// Получить соседние ребра для узла
    /// \param nodeId Идентификатор узла
    /// \param orderBy Сортировка
    /// \param limit Ограничение по количеству, 0 означает, что ограничения нет
    virtual vector <Edge<Value>> getEdgesAtNode(id_t nodeId, ORDER_BY orderBy = ORDER_BY::NOTHING,
                                         size_t limit = 0) {
        throw NotImplemented();
    }

    /// Получить ребра исходящие из узла
    /// \param nodeId Идентификатор узла
    /// \param orderBy Сортировка
    /// \param limit Ограничение по количеству, 0 означает, что ограничени
    virtual vector<Edge<Value>> getOutgoingEdgesAtNode(id_t nodeId, ORDER_BY order, size_t limit) {
        throw NotImplemented();
    }

    /// Получить ребра входящие в узел
    /// \param nodeId Идентификатор узла
    /// \param orderBy Сортировка
    /// \param limit Ограничение по количеству, 0 означает, что ограничени
    virtual vector<Edge<Value>> getIngoingEdgesAtNode(id_t nodeId, ORDER_BY order, size_t limit) {
        throw NotImplemented();
    }

    /// Получить любое ребро соединенное с узлом.
    /// Если соседних ребер нет, бросается исключение NotFound
    /// \param nodeId Идентификатор узла
    /// \throw NotFound В случае отсутствия соседних ребер
    Edge<Value> getAnyEdgeForNode(id_t nodeId) {
        throw NotImplemented();
    }
    /// Получить любое ребро исходящее из узла.
    /// Если соседних ребер нет, бросается исключение NotFound
    /// \param nodeId Идентификатор узла
    /// \throw NotFound В случае отсутствия соседних ребер
    Edge<Value> getAnyOutgoingEdgeAtNode(id_t nodeId) {
        throw NotImplemented();
    }
    /// Получить любое ребро входящее в узел.
    /// Если соседних ребер нет, бросается исключение NotFound.
    /// \param nodeId Идентификатор узла
    /// \throw NotFound В случае отсутствия соседних ребер
    Edge<Value> getAnyIngoingEdgeAtNode(id_t nodeId) {
        throw NotImplemented();
    }

    /// Получить ребро с минимальным весом соединенное с узлом.
    /// Если соседних ребер нет, бросается исключение NotFound.
    /// \param nodeId Идентификатор узла
    /// \throw NotFound В случае отсутствия соседних ребер
    virtual Edge<Value> getMinEdgeAtNode(id_t nodeId) {
        throw NotImplemented();
    }

    /// Получить ребро с максимальным весом соединенное с узлом.
    /// Если соседних ребер нет, бросается исключение NotFound.
    /// \param nodeId Идентификатор узла
    /// \throw NotFound В случае отсутствия соседних ребер
    virtual Edge<Value> getMaxEdgeAtNode(id_t nodeId) {
        throw NotImplemented();
    }

    /// Получить ребро с минимальным весом исходящее из узла.
    /// Если исходящих ребер нет, бросается исключение NotFound.
    /// \param nodeId Идентификатор узла
    /// \throw NotFound В случае отсутствия исходящих ребер
    virtual Edge<Value> getMinOutgoingEdgeAtNode(id_t node_id) {
        throw NotImplemented();
    }

    /// Получить ребро с максимальным весом исходящее из узла.
    /// Если исходящих ребер нет, бросается исключение NotFound.
    /// \param nodeId Идентификатор узла
    /// \throw NotFound В случае отсутствия исходящих ребер
    virtual Edge<Value> getMaxOutgoingEdgeAtNode(id_t node_id) {
        throw NotImplemented();
    }

    /// Получить ребро с минимальным весом входящее из узла.
    /// Если входящих ребер нет, бросается исключение NotFound.
    /// \param nodeId Идентификатор узла
    /// \throw NotFound В случае отсутствия входящих ребер
    virtual Edge<Value> getMinIngoingEdgeAtNode(id_t node_id) {
        throw NotImplemented();
    }

    /// Получить ребро с максимальным весом входящее из узла.
    /// Если входящих ребер нет, бросается исключение NotFound.
    /// \param nodeId Идентификатор узла
    /// \throw NotFound В случае отсутствия входящих ребер
    virtual Edge<Value> getMaxIngoingEdgeAtNode(id_t node_id) {
        throw NotImplemented();
    }


    /// Получить ребро с минимальным id соединенное с узлом.
    /// Если соседних ребер нет, бросается исключение NotFound
    /// \param nodeId Идентификатор узла
    /// \throw NotFound В случае отсутствия соседних ребер
    virtual Edge<Value> getFirstEdgeAtNode(id_t nodeId) {
        throw NotImplemented();
    }

    /// Получить ребро с максимальным id соединенное с узлом.
    /// Если соседних ребер нет, бросается исключение NotFound
    /// \param nodeId Идентификатор узла
    /// \throw NotFound В случае отсутствия соседних ребер
    virtual Edge<Value> getLastEdgeAtNode(id_t nodeId) {
        throw NotImplemented();
    }

    /// Получить ребра с минимальным весом соединенные с узлом.
    /// \param nodeId Идентификатор узла
    /// \param orderBy Сортировка [NOTHING, BY_ID, BY_ID_DESC]
    /// \param limit Ограничение по количеству, 0 означает, что ограничения нет
    virtual vector<Edge<Value>> getMinEdgesAtNode(id_t nodeId, ORDER_BY order, size_t limit) {
        throw NotImplemented();
    }

    /// Получить ребра с максимальным весом соединенные с узлом.
    /// \param nodeId Идентификатор узла
    /// \param orderBy Сортировка [NOTHING, BY_ID, BY_ID_DESC]
    /// \param limit Ограничение по количеству, 0 означает, что ограничения нет
    virtual vector<Edge<Value>> getMaxEdgesAtNode(id_t nodeId, ORDER_BY order, size_t limit) {
        throw NotImplemented();
    }

    /// Получить ребра с минимальным весом исходящее из узла.
    /// \param nodeId Идентификатор узла
    /// \param orderBy Сортировка [NOTHING, BY_ID, BY_ID_DESC]
    /// \param limit Ограничение по количеству, 0 означает, что ограничения нет
    virtual vector<Edge<Value>> getMinOutgoingEdgesAtNode(id_t nodeId, ORDER_BY order, size_t limit) {
        throw NotImplemented();
    }

    /// Получить ребра с максимальным весом исходящее из узла.
    /// \param nodeId Идентификатор узла
    /// \param orderBy Сортировка [NOTHING, BY_ID, BY_ID_DESC]
    /// \param limit Ограничение по количеству, 0 означает, что ограничения нет
    virtual vector<Edge<Value>> getMaxOutgoingEdgesAtNode(id_t nodeId, ORDER_BY order, size_t limit) {
        throw NotImplemented();
    }

    /// Получить ребра с минимальным весом входящее в узел.
    /// \param nodeId Идентификатор узла
    /// \param orderBy Сортировка [NOTHING, BY_ID, BY_ID_DESC]
    /// \param limit Ограничение по количеству, 0 означает, что ограничения нет
    virtual vector<Edge<Value>> getMinIngoingEdgesAtNode(id_t nodeId, ORDER_BY order, size_t limit) {
        throw NotImplemented();
    }

    /// Получить ребра с максимальным весом входящее с узел.
    /// \param nodeId Идентификатор узла
    /// \param orderBy Сортировка [NOTHING, BY_ID, BY_ID_DESC]
    /// \param limit Ограничение по количеству, 0 означает, что ограничения нет
    virtual vector<Edge<Value>> getMaxIngoingEdgesAtNode(id_t nodeId, ORDER_BY order, size_t limit) {
        throw NotImplemented();
    }


    /// Количество ребер в узле
    /// \param nodeId Идентификатор узла
    virtual size_t edgesCountAtNode(id_t nodeId) {
        throw NotImplemented();
    }

    /// Количество ребер исходящих из узла
    /// \param nodeId Идентификатор узла
    virtual size_t outgoingEdgesCountAtNode(id_t nodeId) {
        throw NotImplemented();
    }

    /// Количество ребер входящих в узел
    /// \param nodeId Идентификатор узла
    virtual size_t ingoingEdgesCountAtNode(id_t nodeId) {
        throw NotImplemented();
    }
};


#endif //GRAPH_API_BASEGRAPH_H
