#include <algorithm>
#include <cmath>
#include <concepts>
#include <cstddef>
#include <functional>
#include <generator>
#include <print>
#include <ranges>
#include <sys/types.h>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "graph_processor.hpp"
#include "node_containers.hpp"
#include "node_graph.hpp"


template<>
template<class Container>
    requires(std::derived_from<Container, node_containers::AbstractNodeContainer<Container>>)
std::generator<NodeGraph<>::id_t>
NodeGraphProcessor<>::traverse_helper(NodeGraph<>::id_t start, Container &container) const
{
    std::unordered_set<id_t> discovered;
    container.push(start, 0.0);
    while (!container.empty()) {
        auto node = container.pop();
        if (!discovered.contains(node)) {
            co_yield node;
            discovered.insert(node);
            container.push_range(graph.nodes.at(node)->neighbors);
        }
    }
}
std::vector<typename NodeGraph<>::id_t>
convert(NodeGraph<>::id_t start, NodeGraph<>::id_t end, const std::unordered_map<typename NodeGraph<>::id_t, NodeGraph<>::id_t> &prev)
{
    std::vector<typename NodeGraph<>::id_t> result;
    for (auto node = end; node != start; node = prev.at(node)) {
        result.push_back(node);
    }
    result.push_back(start);
    std::ranges::reverse(result);
    return result;
}

template<>
template<class Container>
    requires(std::derived_from<Container, node_containers::AbstractNodeContainer<Container>>)
std::pair<double, std::vector<NodeGraph<>::id_t>>
NodeGraphProcessor<>::path_helper(
        NodeGraph<>::id_t start,
        NodeGraph<>::id_t end,
        node_containers::AbstractNodeContainer<Container> &container,
        const std::function<
                double(
                        NodeGraph<>::id_t,
                        const std::unordered_map<NodeGraph<>::id_t, double>)>
                &heuristic) const
{
    auto dist = graph.nodes |
                std::ranges::views::transform([](auto &node) {
                    return node.first;
                }) |
                std::ranges::views::transform([](NodeGraph<>::id_t id) {
                    return std::pair<NodeGraph<>::id_t, double>(id, std::numeric_limits<double>::max());
                }) |
                std::ranges::to<std::unordered_map>();
    auto prev = graph.nodes |
                std::ranges::views::transform([](auto &node) {
                    return node.first;
                }) |
                std::ranges::views::transform([](NodeGraph<>::id_t id) {
                    return std::pair<NodeGraph<>::id_t, NodeGraph<>::id_t>(id, -1);
                }) |
                std::ranges::to<std::unordered_map>();
    dist[start] = 0;

    std::unordered_set<id_t> discovered;

    container.push(start);
    while (!container.empty()) {
        auto node = container.pop();
        for (auto neighbor: graph.nodes.at(node)->neighbors) {
            double weight = graph.edges.at({node, neighbor}) + dist[node];
            if (weight < dist[neighbor]) {
                dist[neighbor] = weight;
                prev[neighbor] = node;
            }
            if (!discovered.contains(neighbor)) {
                container.push(neighbor, weight);
            } else {
                discovered.insert(neighbor);
            }
        }
        if (node == end) {
            return {dist[end], convert(start, end, prev)};
        }
    }
    return {-1.0, {}};
}

template<>
template<>
std::generator<id_t>
NodeGraphProcessor<>::traverse<BFS>(NodeGraph<>::id_t start) const
{
    node_containers::StackNodeContainer container;
    for (auto node: traverse_helper(start, container)) {
        co_yield node;
    }
}

template<>
template<>
std::generator<id_t>
NodeGraphProcessor<>::traverse<DFS>(NodeGraph<>::id_t start) const
{
    node_containers::QueueNodeContainer container;
    for (auto node: traverse_helper(start, container)) {
        co_yield node;
    }
}

template<>
template<>
std::pair<double, std::vector<NodeGraph<>::id_t>>
NodeGraphProcessor<>::path<BFS>(NodeGraph<>::id_t start, NodeGraph<>::id_t end) const
{
    node_containers::StackNodeContainer container;
    return path_helper(start, end, container);
}

template<>
template<>
std::pair<double, std::vector<NodeGraph<>::id_t>>
NodeGraphProcessor<>::path<DFS>(NodeGraph<>::id_t start, NodeGraph<>::id_t end) const
{
    node_containers::QueueNodeContainer container;
    return path_helper(start, end, container);
}

template<>
template<>
std::pair<double, std::vector<NodeGraph<>::id_t>>
NodeGraphProcessor<>::path<DIJKSTRA>(NodeGraph<>::id_t start, NodeGraph<>::id_t end) const
{
    node_containers::PriorityQueueNodeContainer container;
    return path_helper(start, end, container);
}

static double
distance(double x1, double y1, double x2, double y2)
{
    double x = x2 - x1;
    double y = y2 - y1;
    return sqrt(x * x + y * y);
}

template<>
template<>
std::pair<double, std::vector<NodeGraph<>::id_t>>
NodeGraphProcessor<>::path<A_STAR>(NodeGraph<>::id_t start, NodeGraph<>::id_t end) const
{
    node_containers::PriorityQueueNodeContainer container;
    return path_helper(start, end, container, [this, start](auto dest, auto dist) {
        return dist[dest] +
               distance(graph.nodes.at(start)->x,
                        graph.nodes.at(start)->y,
                        graph.nodes.at(dest)->x,
                        graph.nodes.at(dest)->y);
    });
}
