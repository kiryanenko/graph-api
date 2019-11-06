//
// Created by kiryanenko on 06.11.19.
//

#ifndef GRAPH_API_NODE_H
#define GRAPH_API_NODE_H



template <class Value>
class Node {
    Graph<Value> *_graph;
    /// Идентификатор узла, значение соответствует значению в БД
    id_t _id = 0;
    /// Узел соответсвует узлу хранящиемуся в БД
    bool _commit = false;


public:
    /// Идентификатор узла. После изменения нужно вызвать метод save() для записи в БД.
    id_t id = 0;
    /// Значение узла. После изменения нужно вызвать метод save() для записи в БД.
    Value value;


    Node(Graph<Value> *graph, Value value) : _graph(graph), value(value) {}
    /// Конструктор узла
    /// \param graph Указатель на граф
    /// \param id Идентификатор узла
    /// \param value Значение узла
    /// \param commit Узел соответствует узлу находящимуся в БД. Не рекомендуется выставлять в true.
    Node(Graph<Value> *graph, id_t id, Value value, bool commit=false) :
            _graph(graph), _id(id), id(id), value(value), _commit(commit) {}


    Graph<Value>* getGraph() const { return _graph; }
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
    Edge<Value> connectWithNode(id_t nodeId, weight_t weight, bool oriented=true);

    /// Получить соседние ребра.
    /// \param orderBy Сортировка
    /// \param limit Ограничение по количеству, 0 означает, что ограничения нет
    vector<Edge<Value>> getEdges(ORDER_BY orderBy = ORDER_BY::NOTHING, size_t limit = 0);

    /// Получить любое соседнее ребро
    /// \throw NotFound В случае отсутствия соседних ребер
    Edge<Value> getAnyEdge();

    /// Получить соседнее ребро с минимальным идентификатором
    /// \throw NotFound В случае отсутствия соседних ребер
    Edge<Value> getFirstEdge();
    /// Получить соседнее ребро с максимальным идентификатором
    /// \throw NotFound В случае отсутствия соседних ребер
    Edge<Value> getLastEdge();

    /// Количество соседних ребер
    size_t edgesCount();
};



#endif //GRAPH_API_NODE_H
