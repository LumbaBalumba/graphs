#include <boost/container_hash/hash.hpp>
#include <concepts>
#include <generator>
#include <unordered_map>
#include <utility>
#include <vector>

#include "node_containers.hpp"
#include "node_graph.hpp"

#pragma once

enum iter_types
{
    BFS,
    DFS,
    DIJKSTRA,
    A_STAR
};

template<class T = bool>
class NodeGraphProcessor
{
private:
    const NodeGraph<T> &graph;

public:
    NodeGraphProcessor(NodeGraph<T> &graph) : graph(graph) {}

private:
    template<class Container>
        requires(std::derived_from<Container,
                                   node_containers::AbstractNodeContainer<Container>>)
    std::generator<NodeGraph<>::id_t>
    traverse_helper(NodeGraph<>::id_t start,
                    Container &container) const;

    template<class Container>
        requires(std::derived_from<Container,
                                   node_containers::AbstractNodeContainer<Container>>)
    std::pair<double, std::vector<NodeGraph<>::id_t>>
    path_helper(
            NodeGraph<>::id_t start,
            NodeGraph<>::id_t end,
            node_containers::AbstractNodeContainer<Container> &container,
            const std::function<
                    double(
                            NodeGraph<>::id_t,
                            const std::unordered_map<NodeGraph<>::id_t, double>)>
                    &heuristic = [](auto dest, auto dist) {
                        return dist[dest];
                    }) const;

public:
    template<iter_types IterType>
    std::generator<id_t>
    traverse(NodeGraph<>::id_t start) const;

    template<iter_types IterType>
    std::pair<double, std::vector<NodeGraph<>::id_t>>
    path(
            NodeGraph<>::id_t start,
            NodeGraph<>::id_t end) const;
};
