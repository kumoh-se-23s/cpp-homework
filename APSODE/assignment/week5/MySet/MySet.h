//
// Created by apsode on 26. 10. 13..
//

#ifndef CPP_HOMEWORK_MYSET_H
#define CPP_HOMEWORK_MYSET_H
#include "arrays.h"

using namespace arrays;
using namespace collections;

template
<typename E = int>
class MySet : public Collection<E> {
    DynamicArray<E> container;

    public:
        MySet() = default;

        MySet(const E *array);

        MySet(const MySet &set);

        ~MySet() override;

        E* begin() override;

        E* end() override;

        const E* begin() const override;

        const E* end() const override;

        int get_size() const override;

        int get_capacity() const override;

        int add(const E &value) override;

        int insert(int index, const E &value) override;

        int remove_at(int index) override;

        bool is_empty() override;

        bool is_contains(const E &value) override;

    private:
};

#endif //CPP_HOMEWORK_MYSET_H
