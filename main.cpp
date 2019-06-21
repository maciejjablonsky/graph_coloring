//
// Created by maciek on 22.05.19.
//

#include <iostream>
#include "Graph.h"

using std::cout;
using std::cin;

int main()
{
    std::ios_base::sync_with_stdio(false);
    int n = 0;
    cin >> n;
    while (n > 0)
    {
        Graph graph;
        graph.load_graph_from_input_stream();

        if (graph.connected_graph())
        {
            if (((graph.get_vertices_number() % 2 == 1) && graph.is_cycle()) || graph.is_complete())
            {
                cout << "True\n";
            }
            else { cout << "False\n"; }
        }
        else
        {
            if (graph.max_chromatic_number_from_subgraphs() > graph.get_max_degree())
            {
                cout << "True\n";
            }
            else
            {
                cout << "False\n";
            }
        }
        --n;
    }
    return 0;
}
