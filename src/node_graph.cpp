#include "node_graph.hpp"

template<>
[[nodiscard]]
bool
NodeGraph<>::has_node(id_t node_id) const noexcept
{
    return nodes.contains(node_id);
}

template<>
void
NodeGraph<>::add_edge(edge_t edge, double weight)
{
    for (auto node_id: {edge.first, edge.second}) {
        if (!has_node(node_id)) {
            throw std::invalid_argument(std::format("Invalid edge: graph does not contain node with id {}", node_id));
        }
    }
    edges[edge] = weight;
    nodes[edge.first]->neighbors.push_back(edge.second);
}
