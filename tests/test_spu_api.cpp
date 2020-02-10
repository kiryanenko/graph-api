//
// Created by a.kiryanenko on 2020-02-04.
//

#include <boost/test/unit_test.hpp>
#include <libspu.hpp>
#include <structure.hpp>


BOOST_AUTO_TEST_SUITE(testSpuApi)

    using namespace SPU;
    using namespace boost;


    BOOST_AUTO_TEST_CASE(test_mask)
    {
        auto mask = FieldsLength<int>::mask(4);
        BOOST_CHECK_EQUAL(mask[0], 0xF);
        BOOST_CHECK_EQUAL(mask[1], 0);

        mask = FieldsLength<int>::mask(32);
        BOOST_CHECK_EQUAL(mask[0], 0xFFFFFFFF);
        BOOST_CHECK_EQUAL(mask[1], 0);

        mask = FieldsLength<int>::mask(36);
        BOOST_CHECK_EQUAL(mask[0], 0xFFFFFFFF);
        BOOST_CHECK_EQUAL(mask[1], 0xF);

        mask = FieldsLength<int>::mask(64);
        BOOST_CHECK_EQUAL(mask[0], 0xFFFFFFFF);
        BOOST_CHECK_EQUAL(mask[1], 0xFFFFFFFF);
    }

BOOST_AUTO_TEST_SUITE_END()
