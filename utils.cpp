//
// Created by a.kiryanenko on 3/10/20.
//

#include "utils.h"

namespace SPU_GRAPH
{
    using namespace SPU;

    const auto MAX_DATA = FieldsLength<int>::mask(64);

    /// Получение свободного ключа между min и max включительно
    /// Возвращает 0, если ключ не найден
    key_t get_free_key(const GraphStructure &structure, key_t min, key_t max)
    {
        if (min > max) {
            return 0;
        }

        if (min == max) {
            auto resp = structure.search(min);
            if (resp.status == ERR) {
                return min;
            }
            return 0;
        }

        auto key = max < MAX_DATA ? max + 1 : max;
        auto resp = structure.nsm(key);

        if (resp.status == ERR || resp.key < min) {
            return min;
        }
        if (resp.key < max) {
            return resp.key + 1;
        }

        if (max == MAX_DATA) {
            resp = structure.search(max);
            if (resp.status == ERR) {
                return max;
            }
        }

        auto half = ((max - min) >> u8(1));
        auto res = get_free_key(structure, min, min + half);
        if (res == key_t(0)) {
            res = get_free_key(structure, min + half + 1, max);
        }
        return res;
    }
}