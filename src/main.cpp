#include <print>
#include <ranges>
#include <tuple>

#include "graph_processor.hpp"
#include "node_graph.hpp"

std::tuple<NodeGraph<PositionMetadata>, int, int>
build_graph()
{
    NodeGraph<PositionMetadata> graph;

    for (auto _: std::ranges::views::iota(0, 6)) {
        graph.add_node();
    }
    graph.add_edge({0, 1});
    graph.add_edge({0, 2}, 1.5);
    graph.add_edge({1, 2});
    graph.add_edge({2, 3});
    graph.add_edge({2, 5}, 2.0);
    graph.add_edge({3, 4});
    graph.add_edge({4, 5});
    return {std::move(graph), 0, 5};
}

int
main()
{
    auto [graph, start, end] = build_graph();
    std::println("Graph generated successfully! Num nodes: {}, num edges: {}", graph.nodes().size(), graph.edges().size());

    DFSProcessor dfs_processor(graph);
    BFSProcessor bfs_processor(graph);
    DijkstraProcessor dijkstra_processor(graph);
    AStarProcessor a_star_processor(graph);

    std::println("{}", *dfs_processor.path(start, end));
    std::println("{}", *bfs_processor.path(start, end));
    std::println("{}", *dijkstra_processor.path(start, end));
    std::println("{}", *a_star_processor.path(start, end));
}
