#ifndef MULTICONNECTNODE_H
#define MULTICONNECTNODE_H
#include "AbstractNode.h"
#include "SingleConnectNode.h"
#include "DoubleConnectNode.h"

template<typename T>
class MultiConnectNode final : public AbstractNode<T, MultiConnectNode<T>> {
    MultiConnectNode m_row_next;
    MultiConnectNode m_row_prev;

    MultiConnectNode m_column_next;
    MultiConnectNode m_column_prev;

    public:
        MultiConnectNode();

        explicit MultiConnectNode(const T &item);

        MultiConnectNode(const MultiConnectNode &row_node, const MultiConnectNode &column_node);

        MultiConnectNode(const T &item,
                         const MultiConnectNode &row_node, const MultiConnectNode &column_node);

        ~MultiConnectNode() override = default;


        MultiConnectNode* row_next() const;

        MultiConnectNode* set_row_next(MultiConnectNode *next_node);

        [[nodiscard]] bool has_row_next() const;

        MultiConnectNode* row_prev() const;

        MultiConnectNode* set_row_prev(MultiConnectNode *prev_node);

        [[nodiscard]] bool has_row_prev() const;

        MultiConnectNode* column_next() const;

        MultiConnectNode* set_column_next(MultiConnectNode *next_node);

        [[nodiscard]] bool has_column_next() const;

        MultiConnectNode* column_prev() const;

        MultiConnectNode* set_column_prev(MultiConnectNode *prev_node);

        [[nodiscard]] bool has_column_prev() const;

    private:
};

#include "MultiConnectNode.tpp"

#endif
//MULTICONNECTNODE_H
