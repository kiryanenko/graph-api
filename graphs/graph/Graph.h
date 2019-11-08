//
// Created by kiryanenko on 03.05.19.
//

#ifndef GRAPH_API_BASEGRAPH_H
#define GRAPH_API_BASEGRAPH_H

#include <vector>
#include "../types.h"
#include "../../exceptions.h"


using namespace std;


template <class Value> class Edge;
template <class Value> class Node;


template <class Value>
class Graph {
protected:
    id_t graph_id;

public:
    Graph<Value>();
    explicit Graph<Value>(id_t id) : graph_id(id) {}
    virtual ~Graph() = default;



    ///////////////  Базовые операции с узлом  //////////////////////////

    /// Создать узел. Узлу будет назначен свободный идентификатор
    /// \param value значение
    /// \return Созданный узел
    virtual Node<Value>& createNode(Value value);

    /// Вставить узел, если узел был, то он обновится
    /// \param id идентификатор
    /// \param value значение
    /// \return Вставленный узел
    virtual Node<Value>& insertNode(id_t id, Value value) = 0;

    /// Получить узел
    /// \param id Идентификатор узла
    /// \return Объект узла
    virtual Node<Value>& getNode(id_t id) = 0;

    /// Изменить узел
    /// \param nodeId Идентификатор узла в БД
    /// \param newId Новый идентификатор узла
    /// \param newValue Новое значение
    /// \throw NotFound Если узел не найден
    /// \return Узел соответствующий БД
    virtual Node<Value>& setNode(id_t nodeId, id_t newId, Value newValue) = 0;

    /// Удалить узел
    /// \param nodeId Идентификатор узла
    virtual void removeNode(id_t nodeId) = 0;



    ///////////////  Базовые операции с ребром  //////////////////////////

    /// Создать ребро. Ребру будет назначен свободный идентификатор
    /// \return Созданный ребро
    virtual Edge<Value>& createEdge();

    /// Вставить ребро
    /// \param id Идентификатор ребра
    /// \return Вставленное ребро
    virtual Edge<Value>& insertEdge(id_t id) = 0;

    /// Получить ребро
    /// \param id Идентификато
    /// \return Объект ребра
    virtual Edge<Value>& getEdge(id_t id) = 0;

    /// Изменить ребро
    /// \param nodeId Идентификатор узла в БД
    /// \param newId Новый идентификатор
    /// \throw NotFound Если узел не найден
    /// \return Узел соответствующий БД
    virtual Edge<Value>& setEdge(id_t edgeId, id_t newId) = 0;

    /// Обновить ребро, если его нет то добавить
    /// \param edgeId Идентификатор
    virtual void removeEdge(id_t edgeId) = 0;



    //////////////////// Операции с узлами и ребрами ////////////////////////////

    /// Создать неориентированное ребро между узлами
    /// \param firstNode Идентификатор первого узла
    /// \param secondNode Идентификатор второго узла
    /// \param oriented Ориентированное ребро
    /// \return Созданное ребро
    virtual Edge<Value>& connectNodes(id_t firstNode, id_t secondNode) = 0;

    /// Получить соседние ребра для узла
    /// \param nodeId Идентификатор узла
    /// \param orderBy Сортировка
    /// \param limit Ограничение по количеству, 0 означает, что ограничения нет
    virtual vector<Edge<Value>>& getEdgesForNode(id_t nodeId, ORDER_BY orderBy, size_t limit) = 0;

    /// Получить любое ребро соединенное с узлом.
    /// Если соседних ребер нет, бросается исключение NotFound
    /// \param nodeId Идентификатор узла
    /// \throw NotFound В случае отсутствия соседних ребер
    virtual Edge<Value>& getAnyEdgeForNode(id_t nodeId);

    /// Получить ребро с минимальным id соединенное с узлом.
    /// Если соседних ребер нет, бросается исключение NotFound
    /// \param nodeId Идентификатор узла
    /// \throw NotFound В случае отсутствия соседних ребер
    virtual Edge<Value>& getFirstEdgeForNode(id_t nodeId);

    /// Получить ребро с максимальным id соединенное с узлом.
    /// Если соседних ребер нет, бросается исключение NotFound
    /// \param nodeId Идентификатор узла
    /// \throw NotFound В случае отсутствия соседних ребер
    virtual Edge<Value>& getLastEdgeForNode(id_t nodeId);

    /// Количество ребер в узле
    /// \param nodeId Идентификатор узла
    virtual size_t edgesCountForNode(id_t nodeId) {
        auto edges = getEdgesForNode(nodeId, ORDER_BY::ID_DESC, 1);
        return edges.size();
    }


    /////////////////////// Операции с графом ////////////////////////////

    /// Получить id графа
    inline id_t getId() {
        return graph_id;
    }

    /// Создать копию графа
    /// \param newGraphId Идентификатор нового графа
    virtual void copy(id_t newGraphId) = 0;

    /// Очистить граф от вершин и ребер
    virtual void clear() = 0;

    /// Удалить граф
    virtual void remove() = 0;

protected:
    virtual id_t getFreeGraphId() = 0;
    virtual id_t getFreeNodeId() = 0;
    virtual id_t getFreeEdgeId() = 0;

    virtual void insertNode(id_t id, Value value) = 0;
};



#endif //GRAPH_API_BASEGRAPH_H
