#include "node_containers.hpp"
#include "node_graph.hpp"

void
node_containers::PriorityQueueNodeContainer::push(node_id_t node_id, double weight)
{
    container_.emplace(weight, node_id);
}

node_id_t
node_containers::PriorityQueueNodeContainer::pop()
{
    auto node_id = container_.top().second;
    container_.pop();
    return node_id;
}

bool
node_containers::PriorityQueueNodeContainer::empty() const
{
    return container_.empty();
}
