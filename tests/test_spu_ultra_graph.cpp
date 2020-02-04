//
// Created by kiryanenko on 11.11.19.
//

#include "../SpuUltraGraph.h"

#define BOOST_TEST_MODULE testSpuUltraGraph

#include <boost/test/unit_test.hpp>
#include <boost/graph/graph_concepts.hpp>
//#include <boost/graph/graph_test.hpp>


BOOST_AUTO_TEST_SUITE(testSuiteSpuUltraGraph)


    using namespace SPU_GRAPH;
    using namespace boost;


    struct Fixture
    {
        Fixture() : graph() {}

        ~Fixture() = default;

        SpuUltraGraph graph;
    };


    BOOST_FIXTURE_TEST_CASE(test_check_concept, Fixture)
    {
//        BOOST_CONCEPT_ASSERT(( MutableGraphConcept<SpuUltraGraph> ));
//        graph_test<SpuUltraGraph> graph_tests;
//        graph_tests.test_add_vertex(graph);
    }


    BOOST_FIXTURE_TEST_CASE(test_add_vertex, Fixture)
    {
        auto id = graph.add_vertex();
        BOOST_CHECK(id > 0);
        BOOST_CHECK_EQUAL(graph.num_vertices(), 1);

        auto id2 = graph.add_vertex();
        BOOST_CHECK(id2 > 0);
        BOOST_CHECK(id2 != id);
        BOOST_CHECK_EQUAL(graph.num_vertices(), 2);

        auto id3 = graph.add_vertex(3);
        BOOST_CHECK_EQUAL(id3, 3);

        auto max_id = graph.add_vertex(268435454);
        auto id4 = graph.add_vertex();
        BOOST_CHECK(id4 > id3);
        BOOST_CHECK(id4 < max_id);
        BOOST_CHECK_EQUAL(graph.num_vertices(), 5);
    }


    BOOST_FIXTURE_TEST_CASE(test_add_edge, Fixture)
    {
        auto id = graph.add_edge();
        BOOST_CHECK(id > 0);
        BOOST_CHECK_EQUAL(graph.num_edges(), 1);

        auto id2 = graph.add_edge();
        BOOST_CHECK(id2 > 0);
        BOOST_CHECK(id2 != id);
        BOOST_CHECK_EQUAL(graph.num_edges(), 2);

        auto id3 = graph.add_edge(3);
        BOOST_CHECK_EQUAL(id3, 3);

        auto max_id = graph.add_edge(268435454);
        auto id4 = graph.add_edge();
        BOOST_CHECK(id4 > id3);
        BOOST_CHECK(id4 < max_id);
        BOOST_CHECK_EQUAL(graph.num_edges(), 5);
    }


BOOST_AUTO_TEST_SUITE_END()