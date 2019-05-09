//
// Created by kiryanenko on 09.05.19.
//

#ifndef GRAPH_API_TYPES_H
#define GRAPH_API_TYPES_H

typedef unsigned int id_t;
typedef unsigned int weight_t;

enum EDGES_ORDER_BY {
    NOTHING,
    ID,
    ID_DESC,
    WEIGHT,
    WEIGHT_DESC
};

#endif //GRAPH_API_TYPES_H
