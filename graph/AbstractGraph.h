//
// Created by kiryanenko on 03.05.19.
//

#ifndef GRAPH_API_BASEGRAPH_H
#define GRAPH_API_BASEGRAPH_H

#include <vector>
#include "types.h"
#include "../exceptions.h"


using namespace std;


template <class Value> class Edge;
template <class Value> class Node;


template <class Value>
class AbstractGraph {
public:
    virtual ~AbstractGraph() = default;

    ///////////////  Базовые операции с узлом  //////////////////////////

    /// Создать узел. Узлу будет назначен свободный идентификатор
    /// \param value значение
    /// \return Созданный узел
    virtual Node <Value> createNode(Value value) {
        auto id = getFreeNodeId();
        putNode(id, value);
    }

    /// Вставить узел, если узел был, то он обновится
    /// \param id идентификатор
    /// \param value значение
    /// \return Вставленный узел
    virtual Node<Value> putNode(id_t id, Value value) = 0;

    /// Вставить узел, если узел был, то он обновится
    /// \param id идентификатор
    /// \param value значение
    /// \return Вставленный узел
    inline Node<Value> insertNode(id_t id, Value value) {
        return putNode(id, value)
    }

    /// Изменить узел
    /// \param nodeId Идентификатор узла в БД
    /// \param newId Новый идентификатор
    /// \param newValue Новое значение
    /// \throw NotFound Если узел не найден
    /// \return Узел соответствующий БД
    virtual Node<Value> setNode(id_t nodeId, id_t newId, Value newValue) = 0;

    /// Обновить узел, если его нет то добавить
    /// \param nodeId Идентификатор узла
    void removeNode(id_t nodeId) = 0;


    ///////////////  Базовые операции с ребром  //////////////////////////

    /// Создать ребро. Ребру будет назначен свободный идентификатор
    /// \return Созданный ребро
    virtual Edge<Value> createEdge() {
        auto id = getFreeEdgeId();
        putEdge(id);
    }

    /// Вставить ребро
    /// \param id идентификатор
    /// \return Вставленное ребро
    virtual Edge<Value> putEdge(id_t id) = 0;

    /// Вставить ребро
    /// \param id идентификатор
    /// \return Вставленное ребро
    inline Edge<Value> insertEdge(id_t id) {
        return putEdge(id, value)
    }

    /// Изменить ребро
    /// \param nodeId Идентификатор узла в БД
    /// \param newId Новый идентификатор
    /// \throw NotFound Если узел не найден
    /// \return Узел соответствующий БД
    virtual Edge<Value> setEdge(id_t edgeId, id_t newId) = 0;

    /// Обновить ребро, если его нет то добавить
    /// \param edgeId Идентификатор
    virtual void removeEdge(id_t edgeId) = 0;



    /// Создать неориентированное ребро между узлами
    /// \param firstNode Идентификатор первого узла
    /// \param secondNode Идентификатор второго узла
    /// \param oriented Ориентированное ребро
    /// \return Созданное ребро
    virtual Edge<Value> connectNodes(id_t firstNode, id_t secondNode) = 0;

    /// Получить соседние ребра для узла
    /// \param nodeId Идентификатор узла
    /// \param orderBy Сортировка
    /// \param limit Ограничение по количеству, 0 означает, что ограничения нет
    virtual vector <Edge<Value>> getEdgesForNode(id_t nodeId, ORDER_BY orderBy = ORDER_BY::NOTHING, size_t limit = 0) = 0;

    /// Получить любое ребро соединенное с узлом.
    /// Если соседних ребер нет, бросается исключение NotFound
    /// \param nodeId Идентификатор узла
    /// \throw NotFound В случае отсутствия соседних ребер
    virtual Edge<Value> getAnyEdgeForNode(id_t nodeId) {
        auto edges = getEdgesForNode(nodeId, NOTHING, 1);
        if (edges.size() > 0) {
            return edges[0];
        } else {
            throw NotFound();
        }
    }

    /// Получить ребро с минимальным id соединенное с узлом.
    /// Если соседних ребер нет, бросается исключение NotFound
    /// \param nodeId Идентификатор узла
    /// \throw NotFound В случае отсутствия соседних ребер
    virtual Edge<Value> getFirstEdgeForNode(id_t nodeId) {
        auto edges = getEdgesForNode(nodeId, ORDER_BY::ID, 1);
        if (edges.size() > 0) {
            return edges[0];
        } else {
            throw NotFound();
        }
    }

    /// Получить ребро с максимальным id соединенное с узлом.
    /// Если соседних ребер нет, бросается исключение NotFound
    /// \param nodeId Идентификатор узла
    /// \throw NotFound В случае отсутствия соседних ребер
    virtual Edge<Value> getLastEdgeForNode(id_t nodeId) {
        auto edges = getEdgesForNode(nodeId, ORDER_BY::ID_DESC, 1);
        if (edges.size() > 0) {
            return edges[0];
        } else {
            throw NotFound();
        }
    }

    /// Количество ребер в узле
    /// \param nodeId Идентификатор узла
    virtual size_t edgesCountAtNode(id_t nodeId) {
        auto edges = getEdgesForNode(nodeId, ORDER_BY::ID_DESC, 1);
        return edges.size();
    }

protected:
    virtual id_t getFreeNodeId() = 0;
    virtual id_t getFreeEdgeId() = 0;
};


#endif //GRAPH_API_BASEGRAPH_H
