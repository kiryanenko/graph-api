//
// Created by kiryanenko on 03.05.19.
//

#ifndef GRAPH_API_BASEGRAPH_H
#define GRAPH_API_BASEGRAPH_H

#include <vector>
#include "exceptions.h"
#include "Edge.h"
#include "Node.h"


using namespace std;


typedef unsigned int id_t;


template <class Value> class Node;


template <class Value>
class BaseGraph {
public:
    virtual ~BaseGraph() {}

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

    /// Создать ребро от узла srcNodeId к узлу dstNodeId
    /// \param srcNodeId Идентификатор узла источника
    /// \param dstNodeId Идентификатор узла цели
    /// \param weight Вес ребра
    /// \param oriented Ориентированное ребро
    /// \return Созданное ребро
    Edge connectNodes(id_t srcNodeId, id_t dstNodeId, weight_t weight, bool oriented) {
        throw NotImplemented();
    }

    /// Получить соседние ребра для узла
    /// \param nodeId Идентификатор узла
    /// \param orderBy Сортировка
    /// \param limit Ограничение по количеству, 0 означает, что ограничения нет
    virtual vector <Edge> getEdgesAtNode(id_t nodeId, Edge::ORDER orderBy = Edge::ORDER::NOTHING,
                                         size_t limit = 0) {
        throw NotImplemented();
    }

    /// Получить ребра исходящие из узла
    /// \param nodeId Идентификатор узла
    /// \param orderBy Сортировка
    /// \param limit Ограничение по количеству, 0 означает, что ограничени
    virtual vector<Edge> getOutgoingEdgesAtNode(id_t nodeId, Edge::ORDER order, size_t limit) {
        throw NotImplemented();
    }

    /// Получить ребра входящие в узел
    /// \param nodeId Идентификатор узла
    /// \param orderBy Сортировка
    /// \param limit Ограничение по количеству, 0 означает, что ограничени
    virtual vector<Edge> getIngoingEdgesAtNode(id_t nodeId, Edge::ORDER order, size_t limit) {
        throw NotImplemented();
    }

    /// Получить любое ребро соединенное с узлом.
    /// Если соседних ребер нет, бросается исключение NotFound
    /// \param nodeId Идентификатор узла
    /// \throw NotFound В случае отсутствия соседних ребер
    Edge getAnyEdgeForNode(id_t nodeId) {
        throw NotImplemented();
    }
    /// Получить любое ребро исходящее из узла.
    /// Если соседних ребер нет, бросается исключение NotFound
    /// \param nodeId Идентификатор узла
    /// \throw NotFound В случае отсутствия соседних ребер
    Edge getAnyOutgoingEdgeAtNode(id_t nodeId) {
        throw NotImplemented();
    }
    /// Получить любое ребро входящее в узел.
    /// Если соседних ребер нет, бросается исключение NotFound.
    /// \param nodeId Идентификатор узла
    /// \throw NotFound В случае отсутствия соседних ребер
    Edge getAnyIngoingEdgeAtNode(id_t nodeId) {
        throw NotImplemented();
    }

    /// Получить ребро с минимальным весом соединенное с узлом.
    /// Если соседних ребер нет, бросается исключение NotFound.
    /// \param nodeId Идентификатор узла
    /// \throw NotFound В случае отсутствия соседних ребер
    virtual Edge getMinEdgeAtNode(id_t nodeId) {
        throw NotImplemented();
    }

    /// Получить ребро с максимальным весом соединенное с узлом.
    /// Если соседних ребер нет, бросается исключение NotFound.
    /// \param nodeId Идентификатор узла
    /// \throw NotFound В случае отсутствия соседних ребер
    virtual Edge getMaxEdgeAtNode(id_t nodeId) {
        throw NotImplemented();
    }

    /// Получить ребро с минимальным весом исходящее из узла.
    /// Если исходящих ребер нет, бросается исключение NotFound.
    /// \param nodeId Идентификатор узла
    /// \throw NotFound В случае отсутствия исходящих ребер
    virtual Edge getMinOutgoingEdgeAtNode(id_t node_id) {
        throw NotImplemented();
    }

    /// Получить ребро с максимальным весом исходящее из узла.
    /// Если исходящих ребер нет, бросается исключение NotFound.
    /// \param nodeId Идентификатор узла
    /// \throw NotFound В случае отсутствия исходящих ребер
    virtual Edge getMaxOutgoingEdgeAtNode(id_t node_id) {
        throw NotImplemented();
    }

    /// Получить ребро с минимальным весом входящее из узла.
    /// Если входящих ребер нет, бросается исключение NotFound.
    /// \param nodeId Идентификатор узла
    /// \throw NotFound В случае отсутствия входящих ребер
    virtual Edge getMinIngoingEdgeAtNode(id_t node_id) {
        throw NotImplemented();
    }

    /// Получить ребро с максимальным весом входящее из узла.
    /// Если входящих ребер нет, бросается исключение NotFound.
    /// \param nodeId Идентификатор узла
    /// \throw NotFound В случае отсутствия входящих ребер
    virtual Edge getMaxIngoingEdgeAtNode(id_t node_id) {
        throw NotImplemented();
    }


    /// Получить ребро с минимальным id соединенное с узлом.
    /// Если соседних ребер нет, бросается исключение NotFound
    /// \param nodeId Идентификатор узла
    /// \throw NotFound В случае отсутствия соседних ребер
    virtual Edge getFirstEdgeAtNode(id_t nodeId) {
        throw NotImplemented();
    }

    /// Получить ребро с максимальным id соединенное с узлом.
    /// Если соседних ребер нет, бросается исключение NotFound
    /// \param nodeId Идентификатор узла
    /// \throw NotFound В случае отсутствия соседних ребер
    virtual Edge getLastEdgeAtNode(id_t nodeId) {
        throw NotImplemented();
    }

    /// Получить ребра с минимальным весом соединенные с узлом.
    /// \param nodeId Идентификатор узла
    /// \param orderBy Сортировка [NOTHING, BY_ID, BY_ID_DESC]
    /// \param limit Ограничение по количеству, 0 означает, что ограничения нет
    virtual vector<Edge> getMinEdgesAtNode(id_t nodeId, Edge::ORDER order, size_t limit) {
        throw NotImplemented();
    }

    /// Получить ребра с максимальным весом соединенные с узлом.
    /// \param nodeId Идентификатор узла
    /// \param orderBy Сортировка [NOTHING, BY_ID, BY_ID_DESC]
    /// \param limit Ограничение по количеству, 0 означает, что ограничения нет
    virtual vector<Edge> getMaxEdgesAtNode(id_t nodeId, Edge::ORDER order, size_t limit) {
        throw NotImplemented();
    }

    /// Получить ребра с минимальным весом исходящее из узла.
    /// \param nodeId Идентификатор узла
    /// \param orderBy Сортировка [NOTHING, BY_ID, BY_ID_DESC]
    /// \param limit Ограничение по количеству, 0 означает, что ограничения нет
    virtual vector<Edge> getMinOutgoingEdgesAtNode(id_t nodeId, Edge::ORDER order, size_t limit) {
        throw NotImplemented();
    }

    /// Получить ребра с максимальным весом исходящее из узла.
    /// \param nodeId Идентификатор узла
    /// \param orderBy Сортировка [NOTHING, BY_ID, BY_ID_DESC]
    /// \param limit Ограничение по количеству, 0 означает, что ограничения нет
    virtual vector<Edge> getMaxOutgoingEdgesAtNode(id_t nodeId, Edge::ORDER order, size_t limit) {
        throw NotImplemented();
    }

    /// Получить ребра с минимальным весом входящее в узел.
    /// \param nodeId Идентификатор узла
    /// \param orderBy Сортировка [NOTHING, BY_ID, BY_ID_DESC]
    /// \param limit Ограничение по количеству, 0 означает, что ограничения нет
    virtual vector<Edge> getMinIngoingEdgesAtNode(id_t nodeId, Edge::ORDER order, size_t limit) {
        throw NotImplemented();
    }

    /// Получить ребра с максимальным весом входящее с узел.
    /// \param nodeId Идентификатор узла
    /// \param orderBy Сортировка [NOTHING, BY_ID, BY_ID_DESC]
    /// \param limit Ограничение по количеству, 0 означает, что ограничения нет
    virtual vector<Edge> getMaxIngoingEdgesAtNode(id_t nodeId, Edge::ORDER order, size_t limit) {
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
