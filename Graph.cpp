//
// Created by maciek on 22.05.19.
//

#include "Graph.h"
#include <iostream>
#include "Stack.h"
#include <cstdio>
Graph::Graph() : adjacency_matrix(nullptr), vertices_number(0) {
    load_graph_from_input_stream();
};

Graph::~Graph(void)
{
    free(adjacency_matrix);
}
bool Graph::load_graph_from_input_stream()
{

    if(std::cin>>vertices_number)
    {
        if (adjacency_matrix)
        {
            free(adjacency_matrix);
        }
        adjacency_matrix = (char*)calloc(sizeof(char), vertices_number * vertices_number + 1);
        std::cin>>adjacency_matrix;
        return true;
    }
    return false;

}

size_t Graph::get_vertices_number(void) const {
    return vertices_number;
}

bool Graph::connected_graph() const {
    bool*visited = (bool*)calloc(sizeof*visited, vertices_number);
    Stack stack;
    int vertex_count = 0;
    visited[0] = true;
    stack.push(0);
    while(stack.empty() == false)
    {
        int current = stack.pop();
        vertex_count++;
        for (int neighbour = 0; neighbour < vertices_number; ++neighbour)
        {
            if(neighbours(current, neighbour) && !visited[neighbour])
            {
                visited[neighbour] = true;
                stack.push(neighbour);
            }
        }
    }
    free(visited);
    return vertex_count == vertices_number;
}

bool Graph::neighbours(size_t current, size_t neighbour) const {
    return adjacency_matrix[current*vertices_number + neighbour] == '1';
}


bool Graph::is_cycle(void) const {
    bool cycle_flag = true;
    for (int y = 0; y < vertices_number; ++y)
    {
        int count = 0;
        for (int x = 0; x < vertices_number; ++x)
        {
            if(neighbours(y, x))
            {
                ++count;
            }
        }
        cycle_flag = count ==2;
        if(!cycle_flag) { break;}
    }
    return cycle_flag;
}

bool Graph::is_complete(void) const {
    int sum = 0;
    for (int i = 0; i < vertices_number * vertices_number; ++i)
    {
        sum += adjacency_matrix[i] == '1' ? 1: 0;
    }
    return sum == vertices_number *(vertices_number - 1) && zeros_on_diagonal();
}

bool Graph::zeros_on_diagonal() const {
    for (int i = 0; i < vertices_number; ++i)
    {
        if(adjacency_matrix[i * (vertices_number + 1)] == '1') { return false;}
    }
    return true;
}