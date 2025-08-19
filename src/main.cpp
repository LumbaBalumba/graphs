#include <print>

#include "graph_processor.hpp"
#include "node_graph.hpp"

NodeGraph<>
build_graph()
{
    NodeGraph graph;

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
