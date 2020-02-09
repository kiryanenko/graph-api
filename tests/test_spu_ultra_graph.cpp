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


    BOOST_FIXTURE_TEST_CASE(test_remove_edge, Fixture)
    {
        auto e = graph.add_edge();
        BOOST_CHECK_EQUAL(graph.num_edges(), 1);
        graph.remove_edge(e);
        BOOST_CHECK_EQUAL(graph.num_edges(), 0);

        graph.remove_edge(e);
        BOOST_CHECK_EQUAL(graph.num_edges(), 0);

        auto v1 = graph.add_vertex();
        auto v2 = graph.add_vertex();
        auto v3 = graph.add_vertex();
        auto e1 = graph.add_edge(v1, v2);
        auto e2 = graph.add_edge(v1, v3);
        BOOST_CHECK_EQUAL(graph.num_edges(), 2);
        graph.remove_edge(e1);
        BOOST_CHECK_EQUAL(graph.num_edges(), 1);
        // TODO: проверить отсутствие соединения между v1 и v2
    }


    BOOST_FIXTURE_TEST_CASE(test_parallel_edges_container, Fixture)
    {
        auto v1 = graph.add_vertex();
        auto v2 = graph.add_vertex();
        auto v3 = graph.add_vertex();
        auto e12_1 = graph.add_weight_edge(101, v1, v2, (weight_t) 1);
        auto e12_2 = graph.add_weight_edge(102, v1, v2, (weight_t) 2);
        auto e21 = graph.add_edge(v2, v1);
        auto e23 = graph.add_edge(v2, v3);
        auto e32 = graph.add_edge(v3, v2);
        auto e13 = graph.add_edge(v1, v3);
        auto e31 = graph.add_edge(v3, v1);

        int test_ids[] = {101, 102};
        int test_weights[] = {1, 2};
        int i = 0;
        for (auto edge : graph.parallel_edges(v1, v2)) {
            BOOST_CHECK_EQUAL(edge.first, test_ids[i]);
            BOOST_CHECK_EQUAL(edge.second, test_weights[i]);
            ++i;
        }
        BOOST_CHECK_EQUAL(i, 2);
    }

    BOOST_FIXTURE_TEST_CASE(test_decrement_parallel_edges_iterator, Fixture)
    {
        auto v1 = graph.add_vertex();
        auto v2 = graph.add_vertex();
        auto v3 = graph.add_vertex();
        auto e12_1 = graph.add_weight_edge(101, v1, v2, (weight_t) 1);
        auto e12_2 = graph.add_weight_edge(102, v1, v2, (weight_t) 2);
        auto e21 = graph.add_edge(v2, v1);
        auto e21_w = graph.add_weight_edge(v2, v1, (weight_t) 5);
        auto e23 = graph.add_edge(v2, v3);
        auto e23_w = graph.add_weight_edge(v2, v3, (weight_t) 5);
        auto e32 = graph.add_edge(v3, v2);
        auto e32_w = graph.add_weight_edge(v3, v2, (weight_t) 4);
        auto e13 = graph.add_edge(v1, v3);
        auto e13_w = graph.add_weight_edge(v1, v3, (weight_t) 5);
        auto e31 = graph.add_edge(v3, v1);
        auto e31_w = graph.add_weight_edge(v3, v1, (weight_t) 4);

        auto iter = graph.parallel_edges(v1, v2).end();
        iter--;
        BOOST_CHECK_EQUAL((*iter).first, 102);
        BOOST_CHECK_EQUAL((*iter).second, 2);
        iter--;
        BOOST_CHECK_EQUAL((*iter).first, 101);
        BOOST_CHECK_EQUAL((*iter).second, 1);
    }

BOOST_AUTO_TEST_SUITE_END()