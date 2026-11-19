#ifndef MULTICONNECTNODE_H
#define MULTICONNECTNODE_H
#include "AbstractNode.h"

template<typename T>
class MultiConnectNode final : public AbstractNode<T, MultiConnectNode<T> > {
    MultiConnectNode *m_row_next;

    MultiConnectNode *m_column_next;
    MultiConnectNode *m_column_prev;

    public:
        MultiConnectNode();

        explicit MultiConnectNode(const T &item);

        MultiConnectNode(const MultiConnectNode &other) = delete;

        MultiConnectNode& operator=(const MultiConnectNode &other) = delete;

        ~MultiConnectNode() override = default;


        MultiConnectNode* row_next() const;

        void set_row_next(MultiConnectNode *node);


        MultiConnectNode* column_next() const;

        void set_column_next(MultiConnectNode *node);

        MultiConnectNode* column_prev() const;

        void set_column_prev(MultiConnectNode *node);
};

#include "MultiConnectNode.tpp"

#endif //MULTICONNECTNODE_H
