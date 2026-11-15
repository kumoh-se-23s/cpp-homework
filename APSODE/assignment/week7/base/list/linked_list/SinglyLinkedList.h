//
// Created by apsode on 26. 11. 14..
//

#ifndef CPP_HOMEWORK_SINGLYLINKEDLIST_H
#define CPP_HOMEWORK_SINGLYLINKEDLIST_H
#include <stdexcept>

#include "../List.h"
#include "../../util/RequireGuard.h"
#include "./node/SingleConnectNode.h"

template<typename E>
class SinglyLinkedList final : public List<E> {
    SingleConnectNode<E> *m_head;
    SingleConnectNode<E> *m_tail;

    int m_size;

    public:
        SinglyLinkedList();

        SinglyLinkedList(const E *elements, int length);

        SinglyLinkedList(const SinglyLinkedList &other);

        ~SinglyLinkedList() override;


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


        SinglyLinkedList& operator=(const SinglyLinkedList &other);

        E& operator[](int index) const override;

        E& operator[](int index) override;

    private:
        void require_valid_index(int index) const;

        SingleConnectNode<E>* node_at(int index) const;

        static void swap(SinglyLinkedList &left_list, SinglyLinkedList &right_list);
};

#include "SinglyLinkedList.tpp"

#endif //CPP_HOMEWORK_SINGLYLINKEDLIST_H
