//
// Created by a.kiryanenko on 2020-02-04.
//

#include <libspu.hpp>
#include <boost/test/unit_test.hpp>


BOOST_AUTO_TEST_SUITE(testDataContainerOperators)

    using namespace SPU;
    using namespace boost;


    BOOST_AUTO_TEST_CASE(test_compare_data_containers)
    {
        data_t a(3);
        data_t b(4);
        BOOST_CHECK(a < b);
        BOOST_CHECK(b > a);

        a = 5;
        b = 5;
        BOOST_CHECK(a == b);

        a[0] = 4;
        a[1] = 1;
        b[0] = 10;
        b[1] = 0;
        BOOST_CHECK(a > b);
    }


    BOOST_AUTO_TEST_CASE(test_addition_data_containers)
    {
        data_t a(3);
        data_t b(4);
        data_t c = a + b;
        BOOST_CHECK_EQUAL((int) c, 7);
    }

    BOOST_AUTO_TEST_CASE(test_overflow_addition_data_containers)
    {
        data_t a(0xFFFFFFFEu);
        data_t b(4);
        data_t c = a + b;
        BOOST_CHECK_EQUAL((int) c[0], 2);
        BOOST_CHECK_EQUAL((int) c[1], 1);
    }


    BOOST_AUTO_TEST_CASE(test_subtract_data_containers)
    {
        data_t a(7);
        data_t b(4);
        data_t c = a - b;
        BOOST_CHECK_EQUAL((int) c, 3);
    }

    BOOST_AUTO_TEST_CASE(test_overflow_subtract_data_containers)
    {
        data_t a(3);
        data_t b(5);
        data_t c = a - b;
        BOOST_CHECK_EQUAL((int) c[0], 0xFFFFFFFEu);
        BOOST_CHECK_EQUAL((int) c[1], 0xFFFFFFFFu);
    }


    BOOST_AUTO_TEST_CASE(test_increment_data_containers)
    {
        data_t a(3);
        a++;
        BOOST_CHECK_EQUAL((int) a, 4);
    }

    BOOST_AUTO_TEST_CASE(test_overflow_increment_data_containers)
    {
        data_t a(0xFFFFFFFFu);
        a++;
        BOOST_CHECK_EQUAL((int) a[0], 0);
        BOOST_CHECK_EQUAL((int) a[1], 1);
    }


    BOOST_AUTO_TEST_CASE(test_decrement_data_containers)
    {
        data_t a(3);
        a--;
        BOOST_CHECK_EQUAL((int) a, 2);
    }

    BOOST_AUTO_TEST_CASE(test_overflow_decrement_data_containers)
    {
        data_t a(0);
        a--;
        BOOST_CHECK_EQUAL((int) a[0], 0xFFFFFFFFu);
        BOOST_CHECK_EQUAL((int) a[1], 0xFFFFFFFFu);
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

BOOST_AUTO_TEST_SUITE_END()
