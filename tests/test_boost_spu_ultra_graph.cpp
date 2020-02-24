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

        SpuUltraGraph::vertex_descriptor v1, v2, v3, v4;
        SpuUltraGraph::edge_descriptor e11, e12_1, e12_2, e13, e21,  e23, e32, e31;

        std::vector<SpuUltraGraph::vertex_descriptor> vertex_set;
        std::vector< std::pair<SpuUltraGraph::vertex_descriptor, SpuUltraGraph::vertex_descriptor> > edge_set;

        Fixture() : graph()  {
            v1 = graph.add_vertex();
            v2 = graph.add_vertex();
            v3 = graph.add_vertex();
            v4 = graph.add_vertex();

            e11 = graph.add_edge(v1, v1);
            e12_1 = graph.add_edge(v1, v2);
            e12_2 = graph.add_edge(v1, v2);
            e13 = graph.add_edge(v1, v3);
            e21 = graph.add_edge(v2, v1);
            e23 = graph.add_edge(v2, v3);
            e31 = graph.add_edge(v3, v1);
            e32 = graph.add_edge(v3, v2);

            vertex_set = {v1, v2, v3, v4};
            edge_set = {
                    {v1, v1},
                    {v1, v2},
                    {v1, v2},
                    {v1, v3},
                    {v2, v1},
                    {v2, v3},
                    {v3, v1},
                    {v3, v2},
            };
        }
        ~Fixture() = default;
    };


    BOOST_AUTO_TEST_CASE(test_check_concept)
    {
        BOOST_CONCEPT_ASSERT(( GraphConcept<SpuUltraGraph> ));
        BOOST_CONCEPT_ASSERT(( MutableGraphConcept<SpuUltraGraph> ));
        BOOST_CONCEPT_ASSERT(( VertexListGraphConcept<SpuUltraGraph> ));
        BOOST_CONCEPT_ASSERT(( EdgeListGraphConcept<SpuUltraGraph> ));
        BOOST_CONCEPT_ASSERT(( IncidenceGraphConcept<SpuUltraGraph> ));
        BOOST_CONCEPT_ASSERT(( BidirectionalGraphConcept<SpuUltraGraph> ));
        BOOST_CONCEPT_ASSERT(( AdjacencyGraphConcept<SpuUltraGraph> ));
    }

    BOOST_FIXTURE_TEST_CASE(test_add_vertex, Fixture)
    {
        graph_tests.test_add_vertex(graph);
    }

    BOOST_FIXTURE_TEST_CASE(test_add_edge, Fixture)
    {
        graph_tests.test_add_edge(v1, v2, graph);
        graph_tests.test_add_edge(v2, v2, graph);
    }

    BOOST_FIXTURE_TEST_CASE(test_remove_uv_edge, Fixture)
    {
//        graph_tests.test_remove_edge(v1, v2, graph);
    }

    BOOST_FIXTURE_TEST_CASE(test_remove_edge, Fixture)
    {
        graph_tests.test_remove_edge(e11, graph);
        graph_tests.test_remove_edge(e12_1, graph);
    }

    BOOST_FIXTURE_TEST_CASE(test_clear_vertex, Fixture)
    {
        graph_tests.test_clear_vertex(v1, graph);
    }


    BOOST_FIXTURE_TEST_CASE(test_incidence_graph, Fixture)
    {
        graph_tests.test_incidence_graph(vertex_set, edge_set, graph);
    }

    BOOST_FIXTURE_TEST_CASE(test_bidirectional_graph, Fixture)
    {
        graph_tests.test_bidirectional_graph(vertex_set, edge_set, graph);
    }

    BOOST_FIXTURE_TEST_CASE(test_adjacency_graph, Fixture)
    {
        graph_tests.test_adjacency_graph(vertex_set, edge_set, graph);
    }


BOOST_AUTO_TEST_SUITE_END()