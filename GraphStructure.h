//
// Created by kiryanenko on 12.11.19.
//

#ifndef GRAPH_API_GRAPHSTRUCTURE_H
#define GRAPH_API_GRAPHSTRUCTURE_H

#include <memory>
#include <spu.h>
#include <structure.hpp>
#include "exceptions.h"

namespace SPU_GRAPH
{
    using namespace SPU;

    class GraphStructure {
        std::shared_ptr<AbstractStructure> _struct;

    public:
        GraphStructure() : _struct(new Structure<>) {}
        explicit GraphStructure(std::shared_ptr<AbstractStructure> structure) : _struct(structure ? structure : std::shared_ptr<AbstractStructure>(new Structure<>)) {}

        void set(std::shared_ptr<Structure<>> structure) { _struct = structure; }
        GraphStructure& operator=(std::shared_ptr<AbstractStructure> structure) { _struct = structure; return *this; }

        inline status_t insert(SPU::key_t key, value_t value = {0}, flags_t flags = NO_FLAGS) {
            return check_spu_resp_status(_struct->insert( key,  value, flags));
        }

        /* Delete */
        inline status_t del(SPU::key_t key, flags_t flags = NO_FLAGS) {
            return check_spu_resp_status(_struct->del( key, flags));
        }

        /* Search */
        inline pair_t search(SPU::key_t key, flags_t flags = P_FLAG) const {
            return check_spu_resp(_struct->search(key, flags));
        }

        /* Min */
        inline pair_t min(flags_t flags = P_FLAG) const {
            return check_spu_resp(_struct->min(flags));
        }

        /* Max */
        inline pair_t max(flags_t flags = P_FLAG) const {
            return check_spu_resp(_struct->max(flags));
        }

        /* Next */
        inline pair_t next(SPU::key_t key, flags_t flags = P_FLAG) const {
            return check_spu_resp(_struct->next(key, flags));
        }

        /* Prev */
        inline pair_t prev(SPU::key_t key, flags_t flags = P_FLAG) const {
            return check_spu_resp(_struct->prev(key, flags));
        }

        /* NSM */
        inline pair_t nsm(SPU::key_t key, flags_t flags = P_FLAG) const {
            return check_spu_resp(_struct->nsm(key, flags));
        }

        /* NGR */
        inline pair_t ngr(SPU::key_t key, flags_t flags = P_FLAG) const {
            return check_spu_resp(_struct->ngr(key, flags));
        }

    private:
        static inline pair_t check_spu_resp(pair_t resp) {
            check_spu_resp_status(resp.status);
            return resp;
        }

        static inline status_t check_spu_resp_status(status_t resp) {
            if (resp == QERR) {
                throw QueueError();
            } else if (resp == OERR) {
                throw CommandOverflowError();
            }
            return resp;
        }
    };
}

#endif //GRAPH_API_GRAPHSTRUCTURE_H
