//
// Created by a.kiryanenko on 2020-02-04.
//

#include <boost/test/unit_test.hpp>
#include <libspu.hpp>
#include <structure.hpp>


BOOST_AUTO_TEST_SUITE(testSpuApi)

    using namespace SPU;
    using namespace boost;

    struct Fixture
    {
        Structure<> structure;

        Fixture() : structure() {}
        ~Fixture() = default;
    };

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

    BOOST_AUTO_TEST_CASE(test_shift_right)
    {
        data_t cont = 0x10;
        auto res = cont << (u8) 4;
        BOOST_CHECK_EQUAL(res[0], 0x100);
        BOOST_CHECK_EQUAL(res[1], 0);

        cont = 0x10;
        res = cont << (u8) 0;
        BOOST_CHECK_EQUAL(res[0], 0x10);
        BOOST_CHECK_EQUAL(res[1], 0);

        cont = 0x10000001;
        res = cont << (u8) 12;
        BOOST_CHECK_EQUAL(res[0], 0x1000);
        BOOST_CHECK_EQUAL(res[1], 0x100);

        cont = 0x1;
        res = cont << (u8) 36;
        BOOST_CHECK_EQUAL(res[0], 0);
        BOOST_CHECK_EQUAL(res[1], 0x10);

        cont = 0x1;
        res = cont << (u8) 32;
        BOOST_CHECK_EQUAL(res[0], 0);
        BOOST_CHECK_EQUAL(res[1], 0x1);

        cont = 0x1;
        res = cont << (u8) 100;
        BOOST_CHECK_EQUAL(res[0], 0);
        BOOST_CHECK_EQUAL(res[1], 0);
    }

    BOOST_AUTO_TEST_CASE(test_shift_left)
    {
        data_t cont = 0x10;
        auto res = cont >> (u8) 4;
        BOOST_CHECK_EQUAL(res[0], 0x1);
        BOOST_CHECK_EQUAL(res[1], 0);

        cont = 0x10;
        res = cont >> (u8) 0;
        BOOST_CHECK_EQUAL(res[0], 0x10);
        BOOST_CHECK_EQUAL(res[1], 0);

        cont[0] = 0x1000;
        cont[1] = 0x10001;
        res = cont >> (u8) 8;
        BOOST_CHECK_EQUAL(res[0], 0x1000010);
        BOOST_CHECK_EQUAL(res[1], 0x100);

        cont[0] = 0;
        cont[1] = 0x100;
        res = cont >> (u8) 36;
        BOOST_CHECK_EQUAL(res[0], 0x10);
        BOOST_CHECK_EQUAL(res[1], 0);

        cont[0] = 0;
        cont[1] = 0x1;
        res = cont >> (u8) 32;
        BOOST_CHECK_EQUAL(res[0], 0x1);
        BOOST_CHECK_EQUAL(res[1], 0);

        cont = 0x1234;
        res = cont >> (u8) 100;
        BOOST_CHECK_EQUAL(res[0], 0);
        BOOST_CHECK_EQUAL(res[1], 0);
    }


    BOOST_FIXTURE_TEST_CASE(test_nsm_command, Fixture) {
        auto resp = structure.nsm(123);
        BOOST_CHECK_EQUAL(resp.status, ERR);

        structure.insert(5, 55);
        resp = structure.nsm(123);
        BOOST_CHECK_EQUAL(resp.status, OK);
        BOOST_CHECK_EQUAL((int) resp.key, 5);
        BOOST_CHECK_EQUAL((int) resp.value, 55);

        resp = structure.nsm(3);
        BOOST_CHECK_EQUAL(resp.status, ERR);
    }

    BOOST_FIXTURE_TEST_CASE(test_prev_command, Fixture) {
        auto resp = structure.prev(123);
        BOOST_CHECK_EQUAL(resp.status, ERR);

        structure.insert(10, 10);
        resp = structure.prev(10);
        BOOST_CHECK_EQUAL(resp.status, ERR);

        structure.insert(5, 55);
        resp = structure.prev(10);
        BOOST_CHECK_EQUAL(resp.status, OK);
        BOOST_CHECK_EQUAL((int) resp.key, 5);
        BOOST_CHECK_EQUAL((int) resp.value, 55);

        resp = structure.prev(123);
        BOOST_CHECK_EQUAL(resp.status, ERR);

        resp = structure.prev(3);
        BOOST_CHECK_EQUAL(resp.status, ERR);
    }

BOOST_AUTO_TEST_SUITE_END()
