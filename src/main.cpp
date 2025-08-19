#include <print>
#include <random>
#include <stack>
#include <tuple>
#include <vector>

#include "graph_processor.hpp"
#include "node_graph.hpp"

constexpr int maze_width = 100;
constexpr int maze_height = 100;

std::tuple<NodeGraph<>, int, int>
build_graph()
{
    NodeGraph graph;
    std::vector<std::vector<bool>> visited(maze_height, std::vector<bool>(maze_width, false));

    std::random_device rd;
    std::mt19937 rng(rd());

    // Map grid coordinates to node IDs
    auto get_node_id = [&](int r, int c) {
        return r * maze_width + c;
    };

    // Add all nodes initially
    for (int r = 0; r < maze_height; ++r) {
        for (int c = 0; c < maze_width; ++c) {
            graph.add_node(static_cast<double>(r), static_cast<double>(c));// Adds a node with a new ID
        }
    }

    std::stack<std::pair<int, int>> stack;
    // Start DFS from a random cell
    std::uniform_int_distribution<> row_dist(0, maze_height - 1);
    std::uniform_int_distribution<> col_dist(0, maze_width - 1);

    int start_row = row_dist(rng);
    int start_col = col_dist(rng);

    stack.push({start_row, start_col});
    visited[start_row][start_col] = true;

    // Define directions (Up, Down, Left, Right)
    std::vector<std::pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    while (!stack.empty()) {
        auto [r, c] = stack.top();
        stack.pop();

        std::shuffle(directions.begin(), directions.end(), rng);

        for (const auto &dir: directions) {
            int nr = r + dir.first;
            int nc = c + dir.second;

            if (nr >= 0 && nr < maze_height && nc >= 0 && nc < maze_width && !visited[nr][nc]) {
                visited[nr][nc] = true;
                // Add edge between current node and new node
                graph.add_edge({get_node_id(r, c), get_node_id(nr, nc)}, 1.0);
                stack.push({nr, nc});
                stack.push({r, c});// Push current cell back to explore other directions
                break;             // Only go one step deep for maze generation
            }
        }
    }

    // Define entrance and exit
    int entrance_node_id = get_node_id(0, 0);                       // Top-left corner
    int exit_node_id = get_node_id(maze_height - 1, maze_width - 1);// Bottom-right corner

    return {std::move(graph), entrance_node_id, exit_node_id};
}

int
main()
{
    auto [graph, start, end] = build_graph();

    auto processor = NodeGraphProcessor(graph);

    std::println("DFS path search: {}", processor.path<DFS>(start, end));
    std::println("DIJKSTRA path search: {}", processor.path<DIJKSTRA>(start, end));
    std::println("A* path search: {}", processor.path<A_STAR>(start, end));
}
