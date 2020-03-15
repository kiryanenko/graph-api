//
// Created by a.kiryanenko on 3/10/20.
//

#ifndef GRAPH_API_UTILS_H
#define GRAPH_API_UTILS_H

#include "StructureIterator.h"

namespace SPU_GRAPH
{
    using namespace SPU;

    extern const data_t MAX_DATA;

    /// Поиск свободного ключа между min и max включительно
    /// \param structure Структура, в которой будет происходить поиск
    /// \param min Нижняя граница ключа
    /// \param max Верхняя граница ключа
    /// \return Свободный ключ
    /// \throw InsufficientStorage если ключ не найден
    SPU::key_t get_free_key(const GraphStructure &structure, SPU::key_t min, SPU::key_t max);

    /// Поиск свободного домена
    /// \param structure Структура, в которой будет происходить поиск
    /// \param prefix Постоянная часть перед доменом
    /// \param prefix_depth Битность префикса
    /// \param domain_depth Битность домена
    /// \param min Минимальный домен включительно
    /// \param max Максмальный домен включительно
    /// \return Свободный домен
    /// \throw InsufficientStorage если домен не найден
    SPU::key_t get_free_domain(const GraphStructure &structure, uint8_t domain_depth, SPU::key_t prefix = 0, uint8_t prefix_depth = 0, SPU::key_t min = 0, SPU::key_t max = MAX_DATA);
}

#endif //GRAPH_API_UTILS_H
