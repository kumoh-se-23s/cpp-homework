//
// Created by apsode on 26. 11. 14..
//

#ifndef CPP_HOMEWORK_DOUBLYLINKEDLIST_H
#define CPP_HOMEWORK_DOUBLYLINKEDLIST_H
#include <stdexcept>

#include "../List.h"
#include "../../util/RequireGuard.h"
#include "./node/DoubleConnectNode.h"

template<typename E>
class DoublyLinkedList final : public List<E> {
    DoubleConnectNode<E> *m_head;

    int m_size;

    public:
        DoublyLinkedList();

        DoublyLinkedList(const E *elements, int length);

        DoublyLinkedList(const DoublyLinkedList &other);

        ~DoublyLinkedList() override;


        void add(const E &element) override;

        void add_all(const E *elements, int length) override;

        void add_first(const E &element) override;

        void add_last(const E &element) override;

        void insert(int index, const E &element);

        const E& peek(int index) const override;

        E get(int index) const override;

        void set(int index, const E &element) override;

        E remove(int index) override;

        void clear();

        [[nodiscard]] int size() const override;

        [[nodiscard]] bool empty() const override;

        bool contains(const E &object) const override;


        DoublyLinkedList& operator=(const DoublyLinkedList &other);

        E& operator[](int index) const override;

        E& operator[](int index) override;

    private:
        void require_valid_index(int index) const;

        DoubleConnectNode<E>* node_at(int index) const;

        void link_first_node(DoubleConnectNode<E> *new_node);

        void link_before(DoubleConnectNode<E> *target_node, DoubleConnectNode<E> *new_node);

        static void swap(DoublyLinkedList &left_list, DoublyLinkedList &right_list);
};

#include "DoublyLinkedList.tpp"

#endif //CPP_HOMEWORK_DOUBLYLINKEDLIST_H
