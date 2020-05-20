//
// Created by a.kiryanenko on 3/29/20.
//

#ifndef GRAPH_API_GRAPHPERFORMANCETEST_H
#define GRAPH_API_GRAPHPERFORMANCETEST_H

#include <fstream>
#include "utils.h"

using namespace std;
using namespace boost;

template <class G>
class GraphPerformanceTest {
    typedef typename graph_traits<G>::vertex_descriptor vertex_t;
    typedef typename graph_traits<G>::edge_descriptor edge_t;

public:
    void (*test_func)(G&) = nullptr;
    string results_file = "results.csv";
    size_t avg_iterations_cnt = 3;

    bool should_fill = true;
    bool is_mutable_test = true;
    pair<edge_t, bool> (*add_edge_func)(vertex_t, vertex_t, G&) = nullptr;

    size_t start_vertices_cnt = 1000;
    size_t inc_vertices_value = 1000;
    size_t end_vertices_cnt = 100000;
    size_t edges_per_vertex = 2;

    GraphPerformanceTest() = default;
    GraphPerformanceTest(void (*test_func)(G&), string results_file) : test_func(test_func), results_file(std::move(results_file)) {}

    void start() {
        print_start_info();

        auto start_time = clock();
        if (should_fill) {
            auto iterations_cnt = size_t(ceil((end_vertices_cnt - start_vertices_cnt + 0.0) / inc_vertices_value));
            vector<pair<size_t, double>> results(iterations_cnt);
            for (size_t i = 0; i < iterations_cnt; ++i) {
                auto vertices_cnt = min(start_vertices_cnt + inc_vertices_value * i, end_vertices_cnt);

                cout << time_info() << "Performance iteration " << i + 1 << '/' << iterations_cnt << endl;
                cout << "---------------------------------------------------------" << endl;
                cout << "Vertices count = " << vertices_cnt << endl;
                cout << "Edges count = " << vertices_cnt * edges_per_vertex << endl;
                cout << "---------------------------------------------------------" << endl << endl;

                auto avg_time = run_avg_tests(vertices_cnt);
                results[i] = {vertices_cnt, avg_time};
            }
            save_results(results);
        } else {
            run_avg_tests();
        }
        auto end_time = clock();

        cout << "=========================================================" << endl;
        cout << time_info() << "All tests completed in " << double(end_time - start_time) / CLOCKS_PER_SEC << " seconds." << endl;
    }

private:
    void print_start_info() {
        cout << time_info() << "Starting performance test." << endl;
        cout << "---------------------------------------------------------" << endl;
        cout << "Results file = " << results_file << endl;
        cout << "Should fill = " << should_fill << endl;
        cout << "Is mutable graph test = " << is_mutable_test << endl << endl;

        if (should_fill) {
            cout << "Start vertices count = " << start_vertices_cnt << endl;
            cout << "Increment vertices value = " << inc_vertices_value << endl;
            cout << "End vertices count = " << end_vertices_cnt << endl << endl;

            cout << "Edges per vertex = " << edges_per_vertex << endl;
            cout << "Start edges count = " << start_vertices_cnt * edges_per_vertex << endl;
            cout << "Increment edges value = " << inc_vertices_value * edges_per_vertex << endl;
            cout << "End edges count = " << end_vertices_cnt * edges_per_vertex << endl << endl;
        }

        auto iterations_cnt = should_fill ? size_t(ceil((end_vertices_cnt - start_vertices_cnt + 0.0) / inc_vertices_value)) : 1;
        cout << "Performance iterations count = " << iterations_cnt << endl;
        cout << "AVG iterations count = " << avg_iterations_cnt << endl;
        cout << "Total tests count = " << iterations_cnt * avg_iterations_cnt << endl;
        cout << "---------------------------------------------------------" << endl << endl << endl;
    }

    void fill(G &graph, size_t vertices_cnt) {
        cout << time_info() << "Start filling a graph..." << endl;

        fill_options<G> opts;
        opts.add_edge_func = add_edge_func;

        auto start_time = clock();
        fill_grid_graph(graph, vertices_cnt, vertices_cnt * edges_per_vertex, opts);
        auto end_time = clock();

        cout << time_info() << "Graph filling completed in " << double(end_time - start_time) / CLOCKS_PER_SEC << " seconds." << endl << endl;
    }

    double run_avg_tests(size_t vertices_cnt = 0) {
        G *graph;
        if (!is_mutable_test) {
            graph = new G;
            if (should_fill) {
                fill(*graph, vertices_cnt);
            }
        }

        cout << time_info() << "Starting " << avg_iterations_cnt << " AVG iterations..." << endl << endl;
        double sum_time = 0;
        auto start_time = clock();
        for (size_t i = 0; i < avg_iterations_cnt; ++i) {
            if (is_mutable_test) {
                graph = new G;
                if (should_fill) {
                    fill(*graph, vertices_cnt);
                }
            }
            auto test_time = run_test(*graph);
            if (is_mutable_test) {
                delete graph;
            }

            cout << time_info() << "Test " << i + 1 << '/' << avg_iterations_cnt << " completed in " << test_time << " seconds." << endl << endl;
            sum_time += test_time;
        }
        auto end_time = clock();
        if (!is_mutable_test) {
            delete graph;
        }
        auto avg_time = sum_time / avg_iterations_cnt;

        cout << time_info() << avg_iterations_cnt << " AVG iterations completed in " << double(end_time - start_time) / CLOCKS_PER_SEC << " seconds." << endl;
        cout << avg_iterations_cnt << " AVG test time = " << avg_time << " seconds." << endl << endl;
        return avg_time;
    }

    double run_test(G &graph) {
        auto start_time = clock();
        test_func(graph);
        auto end_time = clock();
        return double(end_time - start_time) / CLOCKS_PER_SEC;
    }

    string time_info() {
        auto lt = time(nullptr);
        auto ptr = localtime(&lt);
        return asctime(ptr);
    }

    void save_results(vector<pair<size_t, double>> &results) {
        ofstream f(results_file);
        for (auto res : results) {
            f << res.first << ',' << res.second << endl;
        }
        f.close();
    }
};


#endif //GRAPH_API_GRAPHPERFORMANCETEST_H
