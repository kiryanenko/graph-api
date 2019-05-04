//
// Created by kiryanenko on 03.05.19.
//

#ifndef GRAPH_API_EDGE_H
#define GRAPH_API_EDGE_H


class Edge {
public:
    enum ORDER {
        NOTHING,
        BY_ID,
        BY_ID_DESC,
        BY_WEIGHT,
        BY_WEIGHT_DESC
    };
};


#endif //GRAPH_API_EDGE_H
