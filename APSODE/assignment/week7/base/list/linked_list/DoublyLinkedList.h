//
// Created by apsode on 26. 11. 14..
//

#ifndef CPP_HOMEWORK_DOUBLYLINKEDLIST_H
#define CPP_HOMEWORK_DOUBLYLINKEDLIST_H
#include "../List.h"


template<typename T>
class DoublyLinkedList final : public List<T> {
    public:
        void add(const T &element) override;

        void add_all(const T *elements, int length) override;

        void add_first(const T &element) override;

        void add_last(const T &element) override;

        const T & peek(int index) const override;

        T get(int index) const override;

        void set(int index, const T &element) override;

        T remove(int index) override;

        [[nodiscard]] int size() const override;

        [[nodiscard]] bool empty() const override;

        bool contains(const T &object) const override;

        T & operator[](int index) const override;

        T & operator[](int index) override;
}

#endif //CPP_HOMEWORK_DOUBLYLINKEDLIST_H
