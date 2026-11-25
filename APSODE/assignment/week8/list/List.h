//
// Created by apsode on 26. 11. 13..
//

#ifndef CPP_HOMEWORK_LIST_H
#define CPP_HOMEWORK_LIST_H

template<typename E>
class List {
    public:
        virtual ~List() = default;


        virtual void add(const E &element) = 0;

        virtual void add_all(const E *elements, int length) = 0;

        virtual void add_first(const E &element) = 0;

        virtual void add_last(const E &element) = 0;

        virtual const E& peek(int index) const = 0;

        virtual E get(int index) const = 0;

        virtual void set(int index, const E &element) = 0;

        virtual E remove(int index) = 0;

        [[nodiscard]] virtual int size() const = 0;

        [[nodiscard]] virtual bool empty() const = 0;

        virtual bool contains(const E &object) const = 0;


        virtual E& operator[](int index) const = 0;

        virtual E& operator[](int index) = 0;
};

#endif //CPP_HOMEWORK_LIST_H
