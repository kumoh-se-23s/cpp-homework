#ifndef DOUBLECONNECTNODE_H
#define DOUBLECONNECTNODE_H
#include "AbstractNode.h"

template<typename T>
class DoubleConnectNode final : public AbstractNode<T, DoubleConnectNode<T> > {
    DoubleConnectNode *m_next_node;
    DoubleConnectNode *m_prev_node;

    public:
        DoubleConnectNode();

        explicit DoubleConnectNode(const T &item);

        DoubleConnectNode(const T &item, DoubleConnectNode *next_node, DoubleConnectNode *prev_node);

        ~DoubleConnectNode() override = default;


        DoubleConnectNode* next() const;

        DoubleConnectNode* set_next(DoubleConnectNode *next_node);

        [[nodiscard]] bool has_next() const;


        DoubleConnectNode* prev() const;

        DoubleConnectNode* set_prev(DoubleConnectNode *prev_node);

        [[nodiscard]] bool has_prev() const;
};

#include "DoubleConnectNode.tpp"

#endif //DOUBLECONNECTNODE_H
