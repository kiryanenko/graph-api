//
// Created by a.kiryanenko on 2020-02-04.
//

#include "../SpuUltraGraph.h"
#include <boost/test/unit_test.hpp>


BOOST_AUTO_TEST_SUITE(testSpuStructureIterator)


    using namespace SPU_GRAPH;
    using namespace boost;


    struct Fixture
    {
        Fixture() : base_struct(),  structure(&base_struct) {}

        ~Fixture() = default;

        Structure<> base_struct;
        StructureDecorator structure;
    };


    BOOST_FIXTURE_TEST_CASE(test_structure_container, Fixture)
    {
        structure.insert(0, 10);
        structure.insert(1, 11);
        structure.insert(2, 22);
        structure.insert(3, 33);
        structure.insert(5, 55);
        structure.insert(8, 88);

        int test05_keys[] = {0, 1, 2, 3, 5};
        int test05_values[] = {10, 11, 22, 33, 55};
        int i = 0;
        for (pair_t p: StructureRange(&structure, 0, 5)) {
            int v = p.value;
            BOOST_CHECK_EQUAL((int) p.key, test05_keys[i]);
            BOOST_CHECK_EQUAL(v, test05_values[i]);
            ++i;
        }
        BOOST_CHECK_EQUAL(i, 5);

        int test49_keys[] = {5, 8};
        int test49_values[] = {55, 88};
        i = 0;
        for (pair_t p: StructureRange(&structure, 4, 9)) {
            int v = p.value;
            BOOST_CHECK_EQUAL((int) p.key, test49_keys[i]);
            BOOST_CHECK_EQUAL(v, test49_values[i]);
            ++i;
        }
        BOOST_CHECK_EQUAL(i, 2);

        i = 0;
        for (pair_t p: StructureRange(&structure, 6, 7)) {
            ++i;
        }
        BOOST_CHECK_EQUAL(i, 0);

        i = 0;
        for (pair_t p: StructureRange(&structure, 9, 11)) {
            ++i;
        }
        BOOST_CHECK_EQUAL(i, 0);
    }

    BOOST_FIXTURE_TEST_CASE(test_init_structure_iterator, Fixture) {
        structure.insert(1, 11);
        StructureIterator iter(&structure, 0);
        auto pair = *iter;
        BOOST_CHECK_EQUAL((int) pair.key, 1);
        BOOST_CHECK_EQUAL((int) pair.value, 11);
    }

BOOST_AUTO_TEST_SUITE_END()