#include <cmath>
#include <cstddef>
#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

#include <boost/container_hash/hash.hpp>

#pragma once

using node_id_t = std::size_t;

struct EmptyMetadata {
    EmptyMetadata() = default;

    EmptyMetadata(const EmptyMetadata &) = default;

    EmptyMetadata(EmptyMetadata &&) = default;
};

struct PositionMetadata {
    double x;
    double y;

    PositionMetadata() = default;

    PositionMetadata(double x, double y) : x(x), y(y) {}

    PositionMetadata(const PositionMetadata &) = default;

    PositionMetadata(PositionMetadata &&) = default;

    double
    distance(const PositionMetadata &other)
    {
        double x = this->x - other.x;
        double y = this->y - other.y;
        return sqrt(x * x + y * y);
    }
};

template<class NodeMetadata = EmptyMetadata>
class NodeGraph
{
public:
private:
    class IdGenerator
    {
        node_id_t cur_node_id_ = 0;

    public:
        IdGenerator() = default;

        node_id_t
        generate()
        {
            return cur_node_id_++;
        }
    };

    IdGenerator id_gen_;

public:
    struct Node {
        node_id_t id;
        NodeMetadata value;
        std::vector<node_id_t> neighbors;

        Node(node_id_t id, NodeMetadata &&value) : id(id), value(value), neighbors({}) {}
    };

    using edge_t = std::pair<node_id_t, node_id_t>;

protected:
    std::unordered_map<node_id_t, std::unique_ptr<Node>> nodes_;

    std::unordered_map<edge_t, double, boost::hash<edge_t>> edges_;

public:
    NodeGraph() = default;

    NodeGraph(const NodeGraph<NodeMetadata> &) = delete;
    NodeGraph &
    operator=(const NodeGraph<NodeMetadata> &) = delete;

    NodeGraph(NodeGraph<NodeMetadata> &&) noexcept = default;
    NodeGraph &
    operator=(NodeGraph<NodeMetadata> &&) noexcept = default;

    ~NodeGraph() noexcept = default;

    [[nodiscard]]
    const auto &
    nodes() const
    {
        return nodes_;
    }

    [[nodiscard]]
    const auto &
    edges() const
    {
        return edges_;
    }

    template<class... Args>
    void
    add_node(Args &&...args)
    {
        auto id = id_gen_.generate();
        nodes_[id] = std::make_unique<Node>(id, {args...});
    }


    void
    add_node(NodeMetadata arg)
    {
        auto id = id_gen_.generate();
        nodes_[id] = std::make_unique<Node>(id, arg);
    }

    void
    add_node()
    {
        auto id = id_gen_.generate();
        nodes_[id] = std::make_unique<Node>(id, NodeMetadata{});
    }


    [[nodiscard]]
    bool
    has_node(node_id_t node_id) const noexcept
    {
        return nodes_.contains(node_id);
    }


    void
    add_edge(edge_t edge, double weight = 1.0)
    {
        for (auto node_id: {edge.first, edge.second}) {
            if (!has_node(node_id)) {
                throw std::invalid_argument(std::format("Invalid edge: graph does not contain node with id {}", node_id));
            }
        }
        edges_[edge] = weight;
        nodes_[edge.first]->neighbors.push_back(edge.second);
    }
};
