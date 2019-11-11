//
// Created by kiryanenko on 11.11.19.
//

#include "../SpuUltraGraph.h"


#define BOOST_TEST_MODULE testSpuUltraGraph
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>




BOOST_AUTO_TEST_SUITE(testSuiteSpuUltraGraph)


using namespace SPU_GRAPH;


struct Fixture
{
    Fixture() : graph() {}

    ~Fixture() = default;

    SpuUltraGraph graph;
};


BOOST_FIXTURE_TEST_CASE(test_add_vertex, Fixture)
{
    auto id = graph.add_vertex();
    BOOST_CHECK(id > 0);
    BOOST_CHECK_EQUAL(graph.vertices_count(), 1);

    auto id2 = graph.add_vertex();
    BOOST_CHECK(id2 > 0);
    BOOST_CHECK(id2 != id);
    BOOST_CHECK_EQUAL(graph.vertices_count(), 2);
}


BOOST_AUTO_TEST_SUITE_END()