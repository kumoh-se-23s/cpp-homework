#ifndef MULTICONNECTNODE_H
#define MULTICONNECTNODE_H
#include "AbstractNode.h"

template<typename T, typename RowNode, typename ColumnNode>
class MultiConnectNode final : public AbstractNode<T, MultiConnectNode<T, RowNode, ColumnNode>> {
    RowNode *m_row_node;

    ColumnNode *m_column_node;

    public:
        MultiConnectNode();

        ~MultiConnectNode();

    private:
};

#include "MultiConnectNode.tpp"

#endif
//MULTICONNECTNODE_H
