//
// Created by kiryanenko on 03.05.19.
//

#ifndef GRAPH_API_BASEGRAPH_H
#define GRAPH_API_BASEGRAPH_H

#include <vector>
#include "Edge.h"


using namespace std;


enum EXCEPTIONS {
    NOT_FOUND = 404
};

typedef unsigned int id_t;


class BaseGraph {
public:
    /// Получить соседние ребра для узла
    /// \param nodeId Идентификатор узла
    /// \param orderBy Сортировка
    /// \param limit Ограничение по количеству, 0 означает, что ограничения нет
    virtual vector <Edge> getEdgesAtNode(id_t nodeId, Edge::ORDER orderBy = Edge::ORDER::NOTHING,
                                         size_t limit = 0) {
        throw "ERROR! Method getEdgesAtNode not implemented.";
    }

    /// Получить ребра исходящие из узла
    /// \param nodeId Идентификатор узла
    /// \param orderBy Сортировка
    /// \param limit Ограничение по количеству, 0 означает, что ограничени
    virtual vector<Edge> getOutgoingEdgesAtNode(id_t nodeId, Edge::ORDER order, size_t limit) {
        throw "ERROR! Method getOutgoingEdgesAtNode not implemented.";
    }

    /// Получить ребра входящие в узел
    /// \param nodeId Идентификатор узла
    /// \param orderBy Сортировка
    /// \param limit Ограничение по количеству, 0 означает, что ограничени
    virtual vector<Edge> getIngoingEdgesAtNode(id_t nodeId, Edge::ORDER order, size_t limit) {
        throw "ERROR! Method getIngoingEdgesAtNode not implemented.";
    }

    /// Получить любое ребро соединенное с узлом.
    /// Если соседних ребер нет, бросается исключение EXCEPTIONS::NOT_FOUND
    /// \param nodeId Идентификатор узла
    /// \throw EXCEPTIONS::NOT_FOUND В случае отсутствия соседних ребер
    Edge getAnyEdgeForNode(id_t nodeId) {
        throw "ERROR! Method getMaxEdgeAtNode not implemented.";
    }
    /// Получить любое ребро исходящее из узла.
    /// Если соседних ребер нет, бросается исключение EXCEPTIONS::NOT_FOUND
    /// \param nodeId Идентификатор узла
    /// \throw EXCEPTIONS::NOT_FOUND В случае отсутствия соседних ребер
    Edge getAnyOutgoingEdgeAtNode(id_t nodeId) {
        throw "ERROR! Method getAnyOutgoingEdgeAtNode not implemented.";
    }
    /// Получить любое ребро входящее в узел.
    /// Если соседних ребер нет, бросается исключение EXCEPTIONS::NOT_FOUND
    /// \param nodeId Идентификатор узла
    /// \throw EXCEPTIONS::NOT_FOUND В случае отсутствия соседних ребер
    Edge getAnyIngoingEdgeAtNode(id_t nodeId) {
        throw "ERROR! Method getAnyIngoingEdgeAtNode not implemented.";
    }

    /// Получить ребро с минимальным весом соединенное с узлом.
    /// Если соседних ребер нет, бросается исключение EXCEPTIONS::NOT_FOUND
    /// \param nodeId Идентификатор узла
    /// \throw EXCEPTIONS::NOT_FOUND В случае отсутствия соседних ребер
    virtual Edge getMinEdgeAtNode(id_t nodeId) {
        throw "ERROR! Method getMinEdgeAtNode not implemented.";
    }

    /// Получить ребро с максимальным весом соединенное с узлом.
    /// Если соседних ребер нет, бросается исключение EXCEPTIONS::NOT_FOUND
    /// \param nodeId Идентификатор узла
    /// \throw EXCEPTIONS::NOT_FOUND В случае отсутствия соседних ребер
    virtual Edge getMaxEdgeAtNode(id_t nodeId) {
        throw "ERROR! Method getMaxEdgeAtNode not implemented.";
    }


    /// Получить ребро с минимальным id соединенное с узлом.
    /// Если соседних ребер нет, бросается исключение EXCEPTIONS::NOT_FOUND
    /// \param nodeId Идентификатор узла
    /// \throw EXCEPTIONS::NOT_FOUND В случае отсутствия соседних ребер
    virtual Edge getFirstEdgeAtNode(id_t nodeId) {
        throw "ERROR! Method getFirstEdgeAtNode not implemented.";
    }

    /// Получить ребро с максимальным id соединенное с узлом.
    /// Если соседних ребер нет, бросается исключение EXCEPTIONS::NOT_FOUND
    /// \param nodeId Идентификатор узла
    /// \throw EXCEPTIONS::NOT_FOUND В случае отсутствия соседних ребер
    virtual Edge getLastEdgeAtNode(id_t nodeId) {
        throw "ERROR! Method getLastEdgeAtNode not implemented.";
    }

    /// Получить ребра с минимальным весом соединенные с узлом.
    /// \param nodeId Идентификатор узла
    /// \param orderBy Сортировка [NOTHING, BY_ID, BY_ID_DESC]
    /// \param limit Ограничение по количеству, 0 означает, что ограничения нет
    virtual vector<Edge> getMinEdgesAtNode(id_t nodeId, Edge::ORDER order, size_t limit) {
        throw "ERROR! Method getMinEdgesAtNode not implemented.";
    }

    /// Получить ребра с максимальным весом соединенные с узлом.
    /// \param nodeId Идентификатор узла
    /// \param orderBy Сортировка [NOTHING, BY_ID, BY_ID_DESC]
    /// \param limit Ограничение по количеству, 0 означает, что ограничения нет
    virtual vector<Edge> getMaxEdgesAtNode(id_t nodeId, Edge::ORDER order, size_t limit) {
        throw "ERROR! Method getMaxEdgesAtNode not implemented.";
    }
};


#endif //GRAPH_API_BASEGRAPH_H
