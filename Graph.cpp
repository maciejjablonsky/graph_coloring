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

void Graph::set_vertices_number(size_t n)
{
    vertices_number = n;
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

size_t Graph::max_chromatic_number_from_subgraphs()
{
    size_t max_chromatic_number = 0;
    VerticesSet vertices(vertices_number);

    Stack stack;

    while (!vertices.empty())
    {
        bool *b_visited = vertices.get_empty_visited_array();
        size_t vertex = vertices.next_vertex();
        b_visited[vertex] = true;
        if (how_many_neighbours(vertex) > 0)
        {
            stack.push(vertex);
            while (!stack.empty())
            {
                size_t v = stack.pop();
                for (size_t i = 0; i < vertices.get_size(); ++i)
                {
                    size_t u = vertices.next_vertex();
                    if (!b_visited[u] && neighbours(v, u))
                    {
                        b_visited[u] = true;
                        stack.push(u);
                    }
                }
            }
        }
        vertices.go_to(vertex);
        size_t *visited_vertices = vertices.get_numbers_of_visited_vertices(b_visited);
        size_t num_visited_vertices = vertices.count_visited_vertices(b_visited);
        // check if it passes brooks
        if ((num_visited_vertices % 2 == 1 && is_subgraph_cycle(visited_vertices, num_visited_vertices)) ||
            is_subgraph_complete(visited_vertices, vertices.count_visited_vertices(b_visited)))
        {
            // then if subgraph.max_degree + 1 > max_chromatic_number
            size_t max_degree = subgraph_max_degree(visited_vertices, num_visited_vertices);
            if (max_degree + 1 > max_chromatic_number)
            {
                max_chromatic_number = max_degree + 1;
            }
        }

        vertices.delete_visited_vertices(b_visited);
        vertices.free_visited_vertices_numbers(visited_vertices);
        vertices.free_visited(b_visited);

    }
    return max_chromatic_number;
}

bool Graph::is_subgraph_complete(size_t *sub_vertices, size_t n)
{
    size_t sum = 0;
    for (size_t y = 0; y < n; ++y)
    {
        for (size_t x = 0; x < n; ++x)
        {
            sum += adjacency_matrix[sub_vertices[y] * vertices_number + sub_vertices[x]] == '1' ? 1 : 0;
        }

    }
    if (sum != n * (n - 1)) { return false; }
    bool zeros_on_diagonal = true;
    for (size_t i = 0; i < n; ++i)
    {
        if (adjacency_matrix[sub_vertices[i] * (vertices_number + 1)] == '1')
        {
            zeros_on_diagonal = false;
            break;
        }
    }

    return zeros_on_diagonal;
}

bool Graph::is_subgraph_cycle(size_t *sub_vertices, size_t n)
{
    bool cycle_flag = true;
    for (size_t y = 0; y < n; ++y)
    {
        size_t count = 0;
        for (size_t x = 0; x < n; ++x)
        {
            if (neighbours(sub_vertices[y], sub_vertices[x]))
            {
                ++count;
            }
        }
        cycle_flag = count == 2;
        if (!cycle_flag) { break; }
    }
    return cycle_flag;
}

size_t Graph::subgraph_max_degree(size_t *vertices, size_t n)
{
    size_t sub_max_degree = 0;
    for (int y = 0; y < n; ++y)
    {
        size_t current = 0;
        for (int x = 0; x < n; ++x)
        {
            current += adjacency_matrix[vertices[y] * vertices_number + vertices[x]] == '1' ? 1 : 0;
        }
        if (current > sub_max_degree) { sub_max_degree = current; }
    }
    return sub_max_degree;
}