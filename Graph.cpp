//
// Created by maciek on 22.05.19.
//

#include "Graph.h"
#include "Stack.h"
#include "VerticesSet.h"
#include "mem.h"

Graph::Graph() : adjacency_matrix(nullptr), vertices_number(0), max_degree(0)
{
};

Graph::~Graph(void)
{
    free(adjacency_matrix);
}

bool Graph::load_graph_from_input_stream()
{

    if (std::cin >> vertices_number)
    {
        if (adjacency_matrix)
        {
            free(adjacency_matrix);
        }
        adjacency_matrix = calloc(char, vertices_number * vertices_number + 1);
        std::cin >> adjacency_matrix;
        return true;
    }
    return false;

}

size_t Graph::get_vertices_number(void) const
{
    return vertices_number;
}

bool Graph::connected_graph() const
{
    bool *visited = calloc(bool, vertices_number);
    Stack stack;
    size_t vertex_count = 0;
    visited[0] = true;
    stack.push(0);
    while (stack.empty() == false)
    {
        size_t current = stack.pop();
        vertex_count++;
        for (size_t neighbour = 0; neighbour < vertices_number; ++neighbour)
        {
            if (neighbours(current, neighbour) && !visited[neighbour])
            {
                visited[neighbour] = true;
                stack.push(neighbour);
            }
        }
    }
    free(visited);
    return vertex_count == vertices_number;
}

bool Graph::neighbours(size_t current, size_t neighbour) const
{
    return adjacency_matrix[current * vertices_number + neighbour] == '1';
}


bool Graph::is_cycle(void) const
{
    bool cycle_flag = true;
    for (size_t y = 0; y < vertices_number; ++y)
    {
        size_t count = 0;
        for (size_t x = 0; x < vertices_number; ++x)
        {
            if (neighbours(y, x))
            {
                ++count;
            }
        }
        cycle_flag = count == 2;
        if (!cycle_flag) { break; }
    }
    return cycle_flag;
}

bool Graph::is_complete(void) const
{
    size_t sum = 0;
    for (size_t i = 0; i < vertices_number * vertices_number; ++i)
    {
        sum += adjacency_matrix[i] == '1' ? 1 : 0;
    }
    return sum == vertices_number * (vertices_number - 1) && zeros_on_diagonal();
}

bool Graph::zeros_on_diagonal() const
{
    for (size_t i = 0; i < vertices_number; ++i)
    {
        if (adjacency_matrix[i * (vertices_number + 1)] == '1') { return false; }
    }
    return true;
}


void Graph::load_subgraphs(void)
{
    VerticesSet vertices(vertices_number);

    Stack stack;

    while (!vertices.empty())
    {
        bool *visited = vertices.get_empty_visited_array();
        size_t vertex = vertices.next_vertex();
        visited[vertex] = true;
        if (how_many_neighbours(vertex) > 0)
        {
            stack.push(vertex);
            while (!stack.empty())
            {
                size_t v = stack.pop();
                for (size_t i = 0; i < vertices.get_size(); ++i)
                {
                    size_t u = vertices.next_vertex();
                    if (!visited[u] && neighbours(v, u))
                    {
                        visited[u] = true;
                        stack.push(u);
                    }
                }
            }
        }
        vertices.go_to(vertex);
        size_t *visited_vertices_numbers = vertices.get_numbers_of_visited_vertices(visited);

        create_subgraph_from_visited(visited_vertices_numbers, vertices.count_visited_vertices(visited));

        vertices.delete_visited_vertices(visited);
        vertices.free_visited_vertices_numbers(visited_vertices_numbers);
        vertices.free_visited(visited);

    }
}

Graph *Graph::create_subgraph_from_visited(size_t *visited, size_t visited_size)
{
    Graph *new_graph = new Graph();
    new_graph->set_vertices_number(visited_size);
    new_graph->adjacency_matrix = calloc(char, visited_size * visited_size + 1);
    size_t sub_v = (size_t) -1; // i know how it looks, but trust me
    size_t visited_y = 0;
    for (size_t super_v = 0; super_v < vertices_number; ++super_v)
    {
        if (visited[visited_y] == super_v)
        {
            sub_v++;
            size_t sub_u = (size_t) -1; // this looks funny too
            size_t visited_x = 0;
            for (size_t super_u = 0; super_u < vertices_number; ++super_u)
            {
                if (visited[visited_x] == super_u)
                {
                    sub_u++;
                    new_graph->adjacency_matrix[sub_v * visited_size + sub_u] = this->adjacency_matrix[
                            super_v * vertices_number + super_u];
                    visited_x++;
                    if (sub_u + 1 == visited_size) { break; }
                }
            }
            visited_y++;
            if (sub_v + 1 == visited_size) { break; }
        }
    }
    subgraphs.push_back(new_graph);
    return new_graph;
}

void Graph::set_vertices_number(size_t n)
{
    vertices_number = n;
}

size_t Graph::get_subgraphs_number(void)
{
    if (subgraphs.empty())
    {
        load_subgraphs();
    }
    return subgraphs.get_size();
}

Graph *Graph::subgraph(size_t index)
{
    return subgraphs.element_at(index);
}

size_t Graph::get_max_degree(void)
{
    if (max_degree == 0)
    {
        for (int y = 0; y < vertices_number; ++y)
        {
            int current = 0;
            for (int x = 0; x < vertices_number; ++x)
            {
                current += adjacency_matrix[y * vertices_number + x] == '1' ? 1 : 0;
            }
            if (current > max_degree) { max_degree = current; }
        }
    }
    return max_degree;
}

size_t Graph::how_many_neighbours(size_t vertex)
{
    size_t sum = 0;
    for (size_t i = 0; i < vertices_number; ++i)
    {
        sum += adjacency_matrix[vertex * vertices_number + i] == '1' ? 1 : 0;
    }
    return sum;
}
