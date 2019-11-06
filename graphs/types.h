//
// Created by kiryanenko on 09.05.19.
//

#ifndef GRAPH_API_TYPES_H
#define GRAPH_API_TYPES_H

typedef unsigned long id_t;
typedef unsigned long weight_t;

enum ORDER_BY {
    NOTHING,
    ID,
    ID_DESC,
    WEIGHT,
    WEIGHT_DESC
};

#endif //GRAPH_API_TYPES_H
