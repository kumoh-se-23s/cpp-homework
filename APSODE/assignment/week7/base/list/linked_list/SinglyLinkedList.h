//
// Created by apsode on 26. 11. 14..
//

#ifndef CPP_HOMEWORK_SINGLYLINKEDLIST_H
#define CPP_HOMEWORK_SINGLYLINKEDLIST_H
#include "../List.h"
#include "./node/SingleConnectNode.h"

template<typename E>
class SinglyLinkedList final : public List<E> {
    SingleConnectNode<E> *m_head;

    int m_size;

    public:
        SinglyLinkedList();

        SinglyLinkedList(const SinglyLinkedList &other);

        SinglyLinkedList(const E *items);

        ~SinglyLinkedList();


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
#endif //CPP_HOMEWORK_SINGLYLINKEDLIST_H
