//
// Created by kiryanenko on 17.11.19.
//

#ifndef GRAPH_API_STRUCTUREITERATOR_H
#define GRAPH_API_STRUCTUREITERATOR_H

#include <boost/iterator/iterator_facade.hpp>
#include "GraphStructure.h"

namespace SPU_GRAPH {
    using namespace boost;

#define INIT_STATUS 0xCC

    class StructureIterator :
            public iterator_facade<
                    StructureIterator,
                    pair_t,
                    bidirectional_traversal_tag,
                    pair_t,
                    SPU::key_t> {
        friend class iterator_core_access;

        const GraphStructure *_s;
        SPU::pair_t _pair;
        SPU::key_t _end;

    public:
        StructureIterator(const GraphStructure *structure, SPU::key_t key, SPU::key_t end=0, SPU::status_t status=INIT_STATUS) :
                _s(structure), _pair({key, 0, status}), _end(end) {
            if (_pair.status == INIT_STATUS) {
                if (key == data_t(0)) {
                    _pair = _s->search(key);
                    if (_pair.status == ERR) {
                        _pair = _s->ngr(key);
                    }
                } else {
                    _pair = _s->ngr(key - 1);
                }
                if (_pair.status == ERR || (end > data_t(0) && _pair.key > end)) {
                    _pair.key = _end;
                    _pair.status = ERR;
                    _pair.value = 0;
                    return;
                }
            }
        }

        SPU::pair_t dereference() const {
            return _pair;
        }

        bool equal(const StructureIterator &other) const {
            if (_pair.status == INIT_STATUS) **this;
            if (other._pair.status == INIT_STATUS) *other;
            return (_pair.status == ERR && other._pair.status == ERR) ||
                    (_pair.status != ERR && other._pair.status != ERR && _pair.key == other._pair.key);
        }

        void increment() {
            _pair = _s->ngr(_pair.key);

            if (_pair.status == ERR || (_end > data_t(0) && _pair.key > _end)) {
                _pair.key = _end;
                _pair.status = ERR;
                _pair.value = 0;
                return;
            }
        }

        void decrement() {
            _pair = _s->nsm(_pair.key);
        }
    };


    /// Контейнер, кот. содержит ключи в диапозоне [start..end], start и end входят в диаопозон.
    class StructureRange {
        const GraphStructure *_s;
        SPU::key_t _start, _end;

    public:
        typedef StructureIterator iterator;

        StructureRange(const GraphStructure *structure, SPU::key_t start, SPU::key_t end) :
            _s(structure), _start(start), _end(end) {}

        iterator begin() { return {_s, _start, _end}; }
        iterator end() { return {_s, _end, _end, ERR}; }
    };
}

#endif //GRAPH_API_STRUCTUREITERATOR_H
