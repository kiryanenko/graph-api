//
// Created by a.kiryanenko on 3/10/20.
//

#include "utils.h"
#include "exceptions.h"

namespace SPU_GRAPH
{
    using namespace SPU;

    const data_t MAX_DATA = FieldsLength<int>::mask(64);

    // Получение свободного ключа между min и max включительно
    SPU::key_t get_free_key(const GraphStructure &structure, SPU::key_t min, SPU::key_t max)
    {
        if (min == max) {
            auto resp = structure.search(min);
            if (resp.status == ERR) {
                return min;
            }
            throw InsufficientStorage();
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
        try {
            return get_free_key(structure, min, min + half);
        } catch (InsufficientStorage &e) {
            return get_free_key(structure, min + half + 1, max);
        }
    }


    enum DOMAIN_STRUCT {PREFIX_F, DOMAIN_F, ADDRESS_F};

    // Поиск свободного домена
    SPU::key_t get_free_domain(const GraphStructure &structure, uint8_t domain_depth, SPU::key_t prefix, uint8_t prefix_depth, SPU::key_t min, SPU::key_t max)
    {
        const auto max_domain = FieldsLength<int>::mask(domain_depth);
        if (max > max_domain) {
            max = max_domain;
        }
        const uint8_t address_depth = 64 - domain_depth - prefix_depth;
        const auto max_address = FieldsLength<int>::mask(address_depth);
        Fields<DOMAIN_STRUCT> key_f({
                                                  {ADDRESS_F, address_depth},
                                                  {DOMAIN_F, domain_depth},
                                                  {PREFIX_F, prefix_depth},
        });
        key_f[PREFIX_F] = prefix;
        key_f[DOMAIN_F] = max;
        key_f[ADDRESS_F] = max_address;
        SPU::key_t key = key_f;

        if (key < MAX_DATA) {
            ++key;
        }
        auto resp = structure.nsm(key);
        key_f = resp.key;
        auto domain = key_f[DOMAIN_F];

        if (resp.status == ERR || key_f[PREFIX_F] != prefix || domain < min) {
            return min;
        }
        if (domain < max) {
            return ++domain;
        }
        if (key == MAX_DATA) {
            resp = structure.search(key);
            if (resp.status == ERR) {
                return max;
            }
        }
        if (min == max) {
            throw InsufficientStorage();
        }

        auto half = (max - min) >> u8(1);
        try {
            return get_free_domain(structure, domain_depth, prefix, prefix_depth, min, min + half);
        } catch (InsufficientStorage &e) {
            return get_free_domain(structure, domain_depth, prefix, prefix_depth, min + half + 1, max);
        }
    }
}