//
// Created by kiryanenko on 03.05.19.
//

#ifndef GRAPH_API_EDGE_H
#define GRAPH_API_EDGE_H


#include "BaseGraph.h"


template <class Value> class BaseGraph;


template <class Value>
class Edge {
    BaseGraph<Value> *_graph;
    /// Идентификатор узла, значение соответствует значению в БД
    id_t _id = 0;
    /// Ребро соответсвует хранящиемуся в БД
    bool _commit = false;


public:
    /// Идентификатор ребра. После изменения нужно вызвать метод save() для записи в БД.
    id_t id = 0;
    /// Вес ребра. После изменения нужно вызвать метод save() для записи в БД.
    weight_t weight;


    Edge(BaseGraph<Value> *graph, weight_t weight) : _graph(graph), weight(weight) {}
    /// Конструктор ребра
    /// \param graph Указатель на граф
    /// \param id Идентификатор ребра
    /// \param weight Вес ребра
    /// \param commit Узел соответствует узлу находящимуся в БД. Не рекомендуется выставлять в true.
    Edge(BaseGraph<Value> *graph, id_t id, weight_t weight, bool commit=false) :
        _graph(graph), _id(id), id(id), weight(weight), _commit(commit) {}


    BaseGraph<Value>* getGraph() const { return _graph; }
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
};


#endif //GRAPH_API_EDGE_H
