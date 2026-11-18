#include "MultiConnectNode.h"

template<typename T>
MultiConnectNode<T>::MultiConnectNode() {
}

template<typename T>
MultiConnectNode<T>::MultiConnectNode(const T &item) {
}

template<typename T>
MultiConnectNode<T>::MultiConnectNode(const RowNode &row_node, const ColumnNode &column_node) {
}

template<typename T>
MultiConnectNode<T>::MultiConnectNode(const T &item, const RowNode &row_node, const ColumnNode &column_node) {
}

template<typename T>
MultiConnectNode<T>::MultiConnectNode(const MultiConnectNode &other_node) {
}

template<typename T>
MultiConnectNode<T> * MultiConnectNode<T>::next() const {
}

template<typename T>
MultiConnectNode<T> * MultiConnectNode<T>::set_next(MultiConnectNode *next_node) {
}

template<typename T>
bool MultiConnectNode<T>::has_next() const {
}

template<typename T>
MultiConnectNode<T> * MultiConnectNode<T>::prev() const {
}

template<typename T>
MultiConnectNode<T> * MultiConnectNode<T>::set_prev(MultiConnectNode *prev_node) {
}

template<typename T>
bool MultiConnectNode<T>::has_prev() const {
}
