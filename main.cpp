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
            cout << "False\n";
        }
        /* else
         {
             if (subgraph_size > 0 && (((subgraph_size % 2 == 1) && is_cycle(subgraph, subgraph_size)) ||
                                       is_complete(subgraph, subgraph_size)))
             {
                 cout << "True\n";
             }
             else
             {
                 cout << "False\n";
             }
             free(subgraph);*/
        --n;
    }
    return 0;
}
