#include <algorithm>
#include <boost/container_hash/hash.hpp>
#include <concepts>
#include <functional>
#include <generator>
#include <optional>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "node_containers.hpp"
#include "node_graph.hpp"

#pragma once

template<
        class T = EmptyMetadata,
        class Container = node_containers::PriorityQueueNodeContainer>
    requires(std::derived_from<Container, node_containers::AbstractNodeContainer>)
class NodeGraphProcessor
{
private:
    const NodeGraph<T> &graph;
    const std::function<double(node_id_t, node_id_t, const std::unordered_map<node_id_t, double>)> heuristic;

public:
    NodeGraphProcessor(
            const NodeGraph<T> &graph,
            const std::function<
                    double(
                            node_id_t,
                            node_id_t,
                            const std::unordered_map<node_id_t, double>)>
                    &heuristic =
                            [](auto node, auto end, auto dist) {
                                return dist[node];
                            })
        : graph(graph), heuristic(heuristic) {}

protected:
    std::optional<std::pair<double, std::vector<node_id_t>>>
    path_helper(
            node_id_t start,
            node_id_t end,
            const std::function<
                    double(
                            node_id_t,
                            node_id_t,
                            const std::unordered_map<node_id_t, double> &)>
                    &heuristic = [](auto dest, auto end, auto dist) {
                        return dist[dest];
                    }) const;

public:
    std::generator<node_id_t>
    traverse(node_id_t start) const;

    std::optional<std::pair<double, std::vector<node_id_t>>>
    path(node_id_t start, node_id_t end) const
    {
        return path_helper(start, end);
    }
};

template<class T, class Container>
    requires(std::derived_from<Container, node_containers::AbstractNodeContainer>)
std::generator<node_id_t>
NodeGraphProcessor<T, Container>::traverse(node_id_t start) const
{
    Container container;
    std::unordered_set<id_t> discovered;
    container.push(start, 0.0);
    while (!container.empty()) {
        auto node = container.pop();
        if (!discovered.contains(node)) {
            co_yield node;
            discovered.insert(node);
            for (const auto &item: graph.nodes().at(node)->neighbors) {
                container.push(item);
            }
        }
    }
}

static std::vector<node_id_t>
convert(node_id_t start, node_id_t end, const std::unordered_map<node_id_t, node_id_t> &prev)
{
    std::vector<node_id_t> result;
    for (auto node = end; node != start; node = prev.at(node)) {
        result.push_back(node);
    }
    result.push_back(start);
    std::ranges::reverse(result);
    return result;
}

template<class T, class Container>
    requires(std::derived_from<Container, node_containers::AbstractNodeContainer>)
std::optional<std::pair<double, std::vector<node_id_t>>>
NodeGraphProcessor<T, Container>::path_helper(
        node_id_t start,
        node_id_t end,
        const std::function<
                double(
                        node_id_t,
                        node_id_t,
                        const std::unordered_map<node_id_t, double> &)>
                &heuristic) const
{
    auto dist = graph.nodes() |
                std::ranges::views::transform([](auto &node) {
                    return node.first;
                }) |
                std::ranges::views::transform([](node_id_t id) {
                    return std::pair<node_id_t, double>(id, std::numeric_limits<double>::max());
                }) |
                std::ranges::to<std::unordered_map>();
    auto prev = graph.nodes() |
                std::ranges::views::transform([](auto &node) {
                    return node.first;
                }) |
                std::ranges::views::transform([](node_id_t id) {
                    return std::pair<node_id_t, node_id_t>(id, -1);
                }) |
                std::ranges::to<std::unordered_map>();
    dist[start] = 0;

    std::unordered_set<id_t> discovered;
    Container container;

    container.push(start);
    while (!container.empty()) {
        auto node = container.pop();
        for (auto neighbor: graph.nodes().at(node)->neighbors) {
            double weight = graph.edges().at({node, neighbor}) + heuristic(node, end, dist);
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
            return {{dist[end], convert(start, end, prev)}};
        }
    }
    return std::nullopt;
}

template<class T = EmptyMetadata>
using DFSProcessor = NodeGraphProcessor<T, node_containers::StackNodeContainer>;

template<class T = EmptyMetadata>
using BFSProcessor = NodeGraphProcessor<T, node_containers::QueueNodeContainer>;

template<class T = EmptyMetadata>
using DijkstraProcessor = NodeGraphProcessor<T, node_containers::PriorityQueueNodeContainer>;

template<class T>
concept HasDistance = requires(T a, T b) {
    { a.distance(b) } -> std::convertible_to<double>;
};

template<class T = PositionMetadata<2>>
    requires(HasDistance<T>)
class AStarProcessor : public DijkstraProcessor<T>
{
public:
    AStarProcessor(const NodeGraph<T> &graph) : DijkstraProcessor<T>(
                                                        graph,
                                                        [&graph](
                                                                auto node,
                                                                auto end,
                                                                auto dist) {
                                                            auto data1 = graph.nodes().at(node)->value;
                                                            auto data2 = graph.nodes().at(node)->value;
                                                            return dist[node] + data1.distance(data2);
                                                        }) {}
};
