//
// Created by a.kiryanenko on 3/10/20.
//

#ifndef GRAPH_API_UTILS_H
#define GRAPH_API_UTILS_H

#include "StructureIterator.h"

namespace SPU_GRAPH
{
    using namespace SPU;

    /// Получение свободного ключа между min и max включительно
    /// Возвращает 0, если ключ не найден
    key_t get_free_key(const GraphStructure &structure, key_t prefix, uint8_t mask);
}

#endif //GRAPH_API_UTILS_H
