#include <cstddef>
#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

#include <boost/container_hash/hash.hpp>

#pragma once

template<class T = bool>
class NodeGraph
{
public:
    using id_t = std::size_t;

private:
    class IdGenerator
    {
        id_t cur_node_id_ = 0;

    public:
        IdGenerator() = default;

        id_t
        generate()
        {
            return cur_node_id_++;
        }
    };

    IdGenerator id_gen_;

public:
    struct Node {
        id_t id;
        T value;
        std::vector<id_t> neighbors;

        double x;
        double y;

        Node(id_t id, T &&value, double x, double y) : id(id), value(value), neighbors({}), x(x), y(y) {}
    };
    std::unordered_map<id_t, std::unique_ptr<Node>> nodes;

    using edge_t = std::pair<id_t, id_t>;
    std::unordered_map<edge_t, double, boost::hash<edge_t>> edges;

    NodeGraph() = default;

    // Delete copy constructor and copy assignment operator
    NodeGraph(const NodeGraph<T> &) = delete;
    NodeGraph &operator=(const NodeGraph<T> &) = delete;

    NodeGraph(NodeGraph<T> &&) noexcept = default;
    NodeGraph &operator=(NodeGraph<T> &&) noexcept = default;

    ~NodeGraph() noexcept = default;

    template<class... Args>
    void
    add_node(Args &&...args, double x = 0.0, double y = 0.0)
    {
        auto id = id_gen_.generate();
        nodes[id] = std::make_unique<Node>(id, {args...}, x, y);
    }


    void
    add_node(T arg, double x = 0.0, double y = 0.0)
    {
        auto id = id_gen_.generate();
        nodes[id] = std::make_unique<Node>(id, arg);
    }

    void
    add_node(double x = 0.0, double y = 0.0)
    {
        auto id = id_gen_.generate();
        nodes[id] = std::make_unique<Node>(id, T{}, x, y);
    }


    [[nodiscard]]
    bool
    has_node(id_t node_id) const noexcept;

    void
    add_edge(edge_t edge, double weight = 1.0);
};
