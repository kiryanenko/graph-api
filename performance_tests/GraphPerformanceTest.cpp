//
// Created by a.kiryanenko on 3/29/20.
//

#include "GraphPerformanceTest.h"

using namespace std;

template<class G>
void GraphPerformanceTest<G>::start()
{
    print_start_info();


}

template<class G>
void GraphPerformanceTest<G>::print_start_info() {
    cout << "Starting performance test." << endl;
    cout << "---------------------------------------------------------" << endl;
    cout << "Results file = " << results_file << endl;
    cout << "Should fill = " << should_fill << endl << endl;

    if (should_fill) {
        cout << "Start vertices count = " << start_vertices_cnt << endl;
        cout << "Increment vertices value = " << inc_vertices_value << endl;
        cout << "End vertices count = " << end_vertices_cnt << endl << endl;

        cout << "Edges per vertex = " << edges_per_vertex << endl << endl;
        cout << "Start edges count = " << start_vertices_cnt * edges_per_vertex << endl;
        cout << "Increment edges value = " << inc_vertices_value * edges_per_vertex << endl;
        cout << "End edges count = " << end_vertices_cnt * edges_per_vertex << endl;
    }

    auto iterations_cnt = should_fill ? size_t(ceil((end_vertices_cnt - start_vertices_cnt + 0.0) / inc_vertices_value)) : 1;
    cout << "Performance iterations count = " << iterations_cnt << endl;
    cout << "AVG iterations count = " << avg_iterations_cnt << endl;
    cout << "Total tests count = " << iterations_cnt * avg_iterations_cnt << endl;
    cout << "---------------------------------------------------------" << endl << endl;
}
