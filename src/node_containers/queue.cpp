#include "node_containers.hpp"
#include "node_graph.hpp"

void
node_containers::QueueNodeContainer::push(NodeGraph<>::id_t node_id, double)
{
    container_.push(node_id);
}

NodeGraph<>::id_t
node_containers::QueueNodeContainer::pop()
{
    auto node_id = container_.front();
    container_.pop();
    return node_id;
}

bool
node_containers::QueueNodeContainer::empty() const
{
    return container_.empty();
}
