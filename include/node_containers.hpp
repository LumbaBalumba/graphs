#include "node_graph.hpp"
#include <functional>
#include <queue>
#include <ranges>
#include <stack>
#include <utility>
#include <vector>

#pragma once

namespace node_containers
{
    template<class D>
    class AbstractNodeContainer
    {
    public:
        AbstractNodeContainer() = default;

        virtual ~AbstractNodeContainer() = default;

        virtual void
        push(NodeGraph<>::id_t node_id, double = 0.0);

        virtual NodeGraph<>::id_t
        pop();

        virtual bool
        empty() const;

        template<class Range>
            requires(std::ranges::range<Range>)
        void
        push_range(const Range &range)
        {
            static_cast<D *>(this)->template _push_range<Range>(range);
        }
    };

    class StackNodeContainer : public AbstractNodeContainer<StackNodeContainer>
    {
    private:
        std::stack<id_t> container_;

    public:
        StackNodeContainer() = default;
        ~StackNodeContainer() override = default;

        void
        push(NodeGraph<>::id_t node_id, double = 0.0) override;

        NodeGraph<>::id_t
        pop() override;

        bool
        empty() const override;

        template<class Range>
            requires(std::ranges::range<Range>)
        void
        push_range(const Range &range)
        {
            container_.push_range(range);
        }
    };

    class QueueNodeContainer : public AbstractNodeContainer<QueueNodeContainer>
    {
    private:
        std::queue<NodeGraph<>::id_t> container_;

    public:
        QueueNodeContainer() = default;
        ~QueueNodeContainer() override = default;

        void
        push(NodeGraph<>::id_t node_id, double) override;

        NodeGraph<>::id_t
        pop() override;

        bool
        empty() const override;


        template<class Range>
            requires(std::ranges::range<Range>)
        void
        _push_range(const Range &range)
        {
            container_.push_range(range);
        }
    };


    class PriorityQueueNodeContainer : public AbstractNodeContainer<PriorityQueueNodeContainer>
    {
    private:
        using element_t = std::pair<double, id_t>;
        std::priority_queue<element_t, std::vector<element_t>, std::greater<element_t>> container_;

    public:
        PriorityQueueNodeContainer() = default;
        ~PriorityQueueNodeContainer() override = default;

        void
        push(NodeGraph<>::id_t node_id, double weight) override;

        NodeGraph<>::id_t
        pop() override;

        bool
        empty() const override;


        template<class Range>
            requires(std::ranges::range<Range>)
        void
        _push_range(const Range &range)
        {
            container_.push_range(range);
        }
    };
}// namespace node_containers
