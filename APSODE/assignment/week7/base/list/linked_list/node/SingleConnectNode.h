#ifndef SINGLECONNECTNODE_H
#define SINGLECONNECTNODE_H
#include "AbstractNode.h"

template<typename T>
class SingleConnectNode final : public AbstractNode<T, SingleConnectNode<T>> {
    SingleConnectNode<T> *m_next_node;

    public:
        SingleConnectNode() = default;

        SingleConnectNode(const T *item);

        SingleConnectNode(const T *item, const SingleConnectNode<T> *next_node);

        ~SingleConnectNode() = default;


        SingleConnectNode<T>* next() const override;

        SingleConnectNode<T>* set_next(const SingleConnectNode<T> *next_node) override;

        bool has_next() override;
};

#include "AbstractNode.h"

#include "SingleConnectNode.tpp"

#endif //SINGLECONNECTNODE_H
