#ifndef SINGLECONNECTNODE_H
#define SINGLECONNECTNODE_H
#include "AbstractNode.h"

template<typename T>
class SingleConnectNode final : public AbstractNode<T, SingleConnectNode<T> > {
    SingleConnectNode *m_next_node;

    public:
        SingleConnectNode();

        explicit SingleConnectNode(const T &item);

        SingleConnectNode(const T &item, SingleConnectNode *next_node);

        ~SingleConnectNode() override = default;


        SingleConnectNode* next() const;

        SingleConnectNode* set_next(SingleConnectNode *next_node);

        [[nodiscard]] bool has_next() const;
};

#include "SingleConnectNode.tpp"

#endif //SINGLECONNECTNODE_H
