#include <print>
#include <random>
#include <ranges>

#include "graph_processor.hpp"
#include "node_graph.hpp"

NodeGraph<>
build_graph()
{
    NodeGraph graph;

    for (auto _: std::ranges::views::iota(0, 200)) {
        graph.add_node();
    }

    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<> node_ids(0, 199);
    std::uniform_real_distribution<> edge_lengths(1.0, 10.0);


    for (auto _: std::ranges::views::iota(0, 1000)) {
        int left = node_ids(rng);
        int right = node_ids(rng);
        double weight = edge_lengths(rng);
        graph.add_edge({left, right}, weight);
    }

    return graph;
}

int
main()
{
    auto graph = build_graph();

    auto processor = NodeGraphProcessor(graph);

    auto start = 0;
    auto end = 149;

    std::println("DFS path search: {}", processor.path<DFS>(start, end));
    std::println("DIJKSTRA path search: {}", processor.path<DIJKSTRA>(start, end));
    std::println("A* path search: {}", processor.path<A_STAR>(start, end));
}
