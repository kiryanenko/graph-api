//
// Created by kiryanenko on 17.11.19.
//

#ifndef GRAPH_API_STRUCTUREITERATOR_H
#define GRAPH_API_STRUCTUREITERATOR_H

#include <boost/iterator/iterator_facade.hpp>
#include "StructureDecorator.h"

namespace SPU_GRAPH {
    using namespace boost;

    class StructureIterator :
            public iterator_facade<
                    StructureIterator,
                    pair_t,
                    bidirectional_traversal_tag,
                    pair_t,
                    SPU::key_t> {
        friend class iterator_core_access;

        const StructureDecorator *_s;
        SPU::key_t _key;
        pair_t _resp = {0, 0, 0xFF};

    public:
        StructureIterator(const StructureDecorator *structure, SPU::key_t key) : _s(structure), _key(key) {}

        SPU::key_t dereference() {
            if (_resp.status == 0xFF) {
                _resp = _s->search(_key);
            }
            return _resp;
        }

        bool equal(const StructureIterator &other) const {
            return _key == other._key;
        }

        void increment() {
            _resp = _s->ngr(_key);
        }

        void decrement() {
            _resp = _s->nsm(_key);
        }
    };
}

#endif //GRAPH_API_STRUCTUREITERATOR_H
