#ifndef MULTILINKEDLIST_H
#define MULTILINKEDLIST_H
#include <functional>

#include "./node/MultiConnectNode.h"
#include "../../table/RowTable.h"
#include "../../table/ColumnTable.h"
#include "../../../core/TraitsConcept.h"

template<typename T, MultiTraits<T> Traits>
class MultiLinkedList final {
    RowTable<T, Traits> m_rows;
    ColumnTable<T, Traits> m_columns;
    int m_size;

    public:
        MultiLinkedList(int min_row_key, int max_row_key);

        ~MultiLinkedList();

        MultiLinkedList(const MultiLinkedList &other) = delete;

        MultiLinkedList& operator=(const MultiLinkedList &other) = delete;


        bool insert(const T &item);

        bool remove(const T &probe);

        const T* find(const T &probe) const;

        [[nodiscard]] int size() const;

        [[nodiscard]] bool empty() const;

        [[nodiscard]] int column_count() const;

        [[nodiscard]] int min_row_key() const;

        [[nodiscard]] int max_row_key() const;

        void for_each(std::function<void(const T &item)> visit) const;

        void for_each(const T &probe, std::function<void(const T &mate)> visit) const;

    private:
        MultiConnectNode<T>* find_node(const T &probe) const;

        static MultiConnectNode<T>* create_node(const T &item);

        static void destroy_node(MultiConnectNode<T> *node);
};

#include "MultiLinkedList.tpp"

#endif //MULTILINKEDLIST_H
