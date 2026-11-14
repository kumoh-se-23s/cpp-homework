//
// Created by apsode on 26. 11. 14..
//

#ifndef CPP_HOMEWORK_DOUBLYLINKEDLIST_H
#define CPP_HOMEWORK_DOUBLYLINKEDLIST_H
#include "../List.h"
#include "./node/DoubleConnectNode.h"


template<typename E>
class DoublyLinkedList final : public List<E> {
    DoubleConnectNode<E> *m_head;
    DoubleConnectNode<E> *m_tail;
    int m_size;

    public:
        DoublyLinkedList();

        DoublyLinkedList(const DoublyLinkedList &other);

        DoublyLinkedList(const E *items);

        ~DoublyLinkedList();

        void add(const E &element) override;

        void add_all(const E *elements, int length) override;

        void add_first(const E &element) override;

        void add_last(const E &element) override;

        const E& peek(int index) const override;

        E get(int index) const override;

        void set(int index, const E &element) override;

        E remove(int index) override;

        [[nodiscard]] int size() const override;

        [[nodiscard]] bool empty() const override;

        bool contains(const E &object) const override;

        E& operator[](int index) const override;

        E& operator[](int index) override;
};

#endif //CPP_HOMEWORK_DOUBLYLINKEDLIST_H
