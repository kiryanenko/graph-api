//
// Created by a.kiryanenko on 2/17/20.
//


#include "../SpuUltraGraphAdapter.h"


#include <boost/test/unit_test.hpp>
#include <boost/graph/graph_concepts.hpp>
#include "graph_test.h"


BOOST_AUTO_TEST_SUITE(testBoostSpuUltraGraph)


    using namespace SPU_GRAPH;
    using namespace boost;


    struct Fixture
    {
        SpuUltraGraph graph;
        graph_test<SpuUltraGraph> graph_tests;

        Fixture() : graph()  {}
        ~Fixture() = default;
    };


    BOOST_AUTO_TEST_CASE(test_check_concept)
    {
        BOOST_CONCEPT_ASSERT(( MutableGraphConcept<SpuUltraGraph> ));
    }

    BOOST_FIXTURE_TEST_CASE(test_add_vertex, Fixture)
    {
        graph_tests.test_add_vertex(graph);
    }


BOOST_AUTO_TEST_SUITE_END()