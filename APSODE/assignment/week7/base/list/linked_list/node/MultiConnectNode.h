#ifndef MULTICONNECTNODE_H
#define MULTICONNECTNODE_H
#include "AbstractNode.h"
#include "SingleConnectNode.h"
#include "DoubleConnectNode.h"

template<typename T>
class MultiConnectNode final : public AbstractNode<T, MultiConnectNode<T>> {
    SingleConnectNode<T> *m_row_node;
    DoubleConnectNode<T> *m_column_node;

    public:
        MultiConnectNode();

        explicit MultiConnectNode(const T &item);


        MultiConnectNode(const RowNode &row_node, const ColumnNode &column_node);

        MultiConnectNode(const T &item,
                         const RowNode &row_node, const ColumnNode &column_node);

        MultiConnectNode(const MultiConnectNode &other_node);

        ~MultiConnectNode() override = default;


        MultiConnectNode* next() const override;

        MultiConnectNode* set_next(MultiConnectNode *next_node) override;

        [[nodiscard]] bool has_next() const override;

        MultiConnectNode* prev() const;

        MultiConnectNode* set_prev(MultiConnectNode *prev_node);

        [[nodiscard]] bool has_prev() const;

    private:
};

#include "MultiConnectNode.tpp"

#endif
//MULTICONNECTNODE_H
