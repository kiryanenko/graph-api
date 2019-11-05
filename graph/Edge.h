//
// Created by kiryanenko on 03.05.19.
//

#ifndef GRAPH_API_EDGE_H
#define GRAPH_API_EDGE_H


#include "../BaseGraph.h"


template <class Value> class AbstractGraph;
template <class Value> class Node;


template <class Value>
class Edge {
    AbstractGraph<Value> *_graph;
    /// Идентификатор узла, значение соответствует значению в БД
    id_t _id = 0;
    /// Ребро соответсвует хранящиемуся в БД
    bool _commit = false;


public:
    /// Идентификатор ребра. После изменения нужно вызвать метод save() для записи в БД.
    id_t id = 0;
    /// Вес ребра. После изменения нужно вызвать метод save() для записи в БД.
    weight_t weight;


    Edge(AbstractGraph<Value> *graph, weight_t weight) : _graph(graph), weight(weight) {}
    /// Конструктор ребра
    /// \param graph Указатель на граф
    /// \param id Идентификатор ребра
    /// \param weight Вес ребра
    /// \param commit Узел соответствует узлу находящимуся в БД. Не рекомендуется выставлять в true.
    Edge(AbstractGraph<Value> *graph, id_t id, weight_t weight, bool commit=false) :
        _graph(graph), _id(id), id(id), weight(weight), _commit(commit) {}


    AbstractGraph<Value>* getGraph() const { return _graph; }
    int getId() const { return id; }
    weight_t getWeight() const { return weight; }


    /// Изменить id ребра
    /// \param newId Новый идентификтор ребра
    /// \param commit Записать измения в БД
    void setId(id_t newId, bool commit=false);
    /// Изменить вес ребра
    /// \param newId Новое значение
    /// \param commit Записать измения в БД
    void setValue(weight_t newWeight, bool commit=false);

    /// Сохранить измения в БД
    void save();
    /// Удалить ребро
    void remove();


    /// Добавить узел. Узел будет добавлен как источник - приемник
    /// \throw MethodNotAllowed Если ребро не связано с БД. Вероятно нужно перед выполнить save().
    void addNode(id_t nodeId);
    /// Добавить узел. Узел будет добавлен как источник - приемник
    /// \throw MethodNotAllowed Если ребро не связано с БД. Вероятно нужно перед выполнить save().
    void addNode(Node<Value> node) { addNode(node.id); }
    /// Добавить узел источник
    /// \throw MethodNotAllowed Если ребро не связано с БД. Вероятно нужно перед выполнить save().
    void addSrcNode(id_t nodeId);
    /// Добавить узел источник
    /// \throw MethodNotAllowed Если ребро не связано с БД. Вероятно нужно перед выполнить save().
    void addSrcNode(Node<Value> node) { addSrcNode(node.id); }
    /// Добавить узел приемник
    /// \throw MethodNotAllowed Если ребро не связано с БД. Вероятно нужно перед выполнить save().
    void addDstNode(id_t nodeId);
    /// Добавить узел приемник
    /// \throw MethodNotAllowed Если ребро не связано с БД. Вероятно нужно перед выполнить save().
    void addDstNode(Node<Value> node) { addDstNode(node.id); }


    /// Получить все соединяемые узлы
    /// \param order Сортировка
    /// \param limit Ограничение по количеству, 0 означает, что ограничения нет
    /// \param offset Смещение относительно начала получаемого списка
    vector<Node<Value>> getNodes(ORDER_BY order = ORDER_BY::NOTHING, size_t limit = 0, size_t offset = 0);

    /// Получить узел от которого идет ребро.
    /// Если таких узлов несколько, то вернет любое из них
    Node<Value> getSrcNode();
    /// Получить узлы от которых идет данное ребро.
    /// \param order Сортировка
    /// \param limit Ограничение по количеству, 0 означает, что ограничения нет
    /// \param offset Смещение относительно начала получаемого списка
    vector<Node<Value>> getSrcNodes(ORDER_BY order = ORDER_BY::NOTHING, size_t limit = 0, size_t offset = 0);

    /// Получить узел к которому идет ребро.
    /// Если таких узлов несколько, то вернет любое из них
    Node<Value> getDstNode();
    /// Получить узлы к которым идет данное ребро.
    /// \param order Сортировка
    /// \param limit Ограничение по количеству, 0 означает, что ограничения нет
    /// \param offset Смещение относительно начала получаемого списка
    vector<Node<Value>> getDstNodes(ORDER_BY order = ORDER_BY::NOTHING, size_t limit = 0, size_t offset = 0);
};


#endif //GRAPH_API_EDGE_H
