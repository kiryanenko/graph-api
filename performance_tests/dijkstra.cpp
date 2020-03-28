//
// Created by a.kiryanenko on 3/26/20.
//

#include "../SpuUltraGraphAdapter.h"
#include "../SpuUltraGraphProperty.h"
#include "utils.h"


using namespace SPU_GRAPH;
using namespace boost;


int main()
{
    SpuUltraGraph graph;
    fill_graph(graph, 100, 200);
    return 0;
}