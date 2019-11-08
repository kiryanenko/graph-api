//
// Created by kiryanenko on 03.05.19.
//
#include <iostream>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/graphviz.hpp>

#include "SpuUltraGraph.h"

using namespace std;
using namespace boost;
using namespace SPU_GRAPH;

int main() {
    BOOST_CONCEPT_ASSERT(( MutableGraphConcept<SpuUltraGraph> ));

    return 0;
}