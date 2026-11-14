#ifndef DOUBLECONNECTNODE_H
#define DOUBLECONNECTNODE_H
#include "AbstractNode.h"

template<typename T>
class DoubleConnectNode final : public AbstractNode<T, DoubleConnectNode<T> > {
    DoubleConnectNode<T> *m_next_node;
    DoubleConnectNode<T> *m_prev_node;

    public:
        DoubleConnectNode() = default;

        DoubleConnectNode(const T *item_ptr);

        DoubleConnectNode(const T *item_ptr, DoubleConnectNode<T> *next_node, DoubleConnectNode<T> *prev_node);

        ~DoubleConnectNode() = default;


        DoubleConnectNode<T>* next() const override;

        DoubleConnectNode<T>* set_next(const DoubleConnectNode<T> *next_node) override;

        bool has_next() override;


        DoubleConnectNode<T>* prev() const;

        DoubleConnectNode<T>* set_prev(const DoubleConnectNode<T> *prev_node);

        bool has_prev() const;

    private:
};

#include "DoubleConnectNode.tpp"

#endif //DOUBLECONNECTNODE_H
