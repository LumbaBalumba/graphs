#include "node_graph.hpp"
#include <functional>
#include <queue>
#include <stack>
#include <utility>
#include <vector>

#pragma once

namespace node_containers
{
    class AbstractNodeContainer
    {
    public:
        AbstractNodeContainer() = default;

        virtual ~AbstractNodeContainer() = default;

        virtual void
        push(node_id_t node_id, double = 0.0)
        {}

        virtual node_id_t
        pop()
        {
            return 1;
        }

        virtual bool
        empty() const
        {
            return true;
        }
    };

    class StackNodeContainer : public AbstractNodeContainer
    {
    private:
        std::stack<id_t> container_;

    public:
        StackNodeContainer() = default;
        ~StackNodeContainer() override = default;

        void
        push(node_id_t node_id, double = 0.0) override;

        node_id_t
        pop() override;

        bool
        empty() const override;
    };

    class QueueNodeContainer : public AbstractNodeContainer
    {
    private:
        std::queue<node_id_t> container_;

    public:
        QueueNodeContainer() = default;
        ~QueueNodeContainer() override = default;

        void
        push(node_id_t node_id, double = 0.0) override;

        node_id_t
        pop() override;

        bool
        empty() const override;
    };


    class PriorityQueueNodeContainer : public AbstractNodeContainer
    {
    private:
        using element_t = std::pair<double, id_t>;
        std::priority_queue<element_t, std::vector<element_t>, std::greater<element_t>> container_;

    public:
        PriorityQueueNodeContainer() = default;
        ~PriorityQueueNodeContainer() override = default;

        void
        push(node_id_t node_id, double weight = 0.0) override;

        node_id_t
        pop() override;

        bool
        empty() const override;
    };
}// namespace node_containers
