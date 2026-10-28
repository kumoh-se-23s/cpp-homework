//
// Created by apsode on 26. 10. 13..
//

#ifndef CPP_HOMEWORK_MYSET_H
#define CPP_HOMEWORK_MYSET_H
#include "collections.h"
#include "lists.h"

using namespace collections;

template<typename E = int>
class MySet : public List<E>{
    List<E> container;

    public:
        E * begin() override;

        E * end() override;

        const E * begin() const override;

        const E * end() const override;

        int get_size() const override;

        int get_capacity() const override;

        int add(const E &value) override;

        int insert(int index, const E &value) override;

        int remove_at(int index) override;

        bool is_empty() override;

        bool is_contains(const E &value) override;

        const E & get(int index) override;

        int set(int index, const E &value) override;

        int append(const E &value) override;

        int add_first(const E &value) override;

        int add_last(const E &value) override;

        ~MySet() override;
};




#endif //CPP_HOMEWORK_MYSET_H
