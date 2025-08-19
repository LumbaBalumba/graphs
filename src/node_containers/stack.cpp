#include "node_containers.hpp"
#include "node_graph.hpp"

void
node_containers::StackNodeContainer::push(NodeGraph<>::id_t node_id, double)
{
    container_.push(node_id);
}

NodeGraph<>::id_t
node_containers::StackNodeContainer::pop()
{
    auto node_id = container_.top();
    container_.pop();
    return node_id;
}

bool
node_containers::StackNodeContainer::empty() const
{
    return container_.empty();
}
