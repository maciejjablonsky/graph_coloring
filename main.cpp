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
        /*cout << graph.get_vertices_number() << ' ' << graph.adjacency_matrix << '\n';
        n--;
        continue;*/

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
            graph.load_subgraphs();
            size_t subgraphs_number = graph.get_subgraphs_number();
            size_t max_chromatic_number = 0;
            for (size_t i = 0; i < subgraphs_number; ++i)
            {
                Graph *subgraph = graph.subgraph(i);
                if (((subgraph->get_vertices_number() % 2 == 1) && subgraph->is_cycle()) || subgraph->is_complete())
                {
                    if (subgraph->get_max_degree() + 1 > max_chromatic_number)
                    {
                        max_chromatic_number = subgraph->get_max_degree() + 1;
                    }
                }
            }
            if (max_chromatic_number > graph.get_max_degree())
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
