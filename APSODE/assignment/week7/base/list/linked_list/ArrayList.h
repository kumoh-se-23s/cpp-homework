#ifndef ARRAYLIST_H
#define ARRAYLIST_H
#include <stdexcept>

#include "../List.h"

template<typename E>
class ArrayList final : public List<E> {
    constexpr static int DEFAULT_CAPACITY = 4;
    // 0.25 => 사용중인 공간이 전체 공간의 25% 일때, 압축
    constexpr static double DEFAULT_SHRINK_CONDITION = 0.25;
    constexpr static double DEFAULT_SHRINK_RATIO = 0.5;
    constexpr static int DEFAULT_EXPAND_RATIO = 2;

    E *container;
    int m_size;
    int m_capacity;

    public:
        ArrayList();

        explicit ArrayList(int manual_capacity);

        ArrayList(const E *elements, int count);

        ArrayList(const ArrayList &other);

        ArrayList& operator=(const ArrayList &other);

        ~ArrayList() override;

        E get(int index) const override;

        void set(int index, const E &other) override;

        const E& peek(int index) const override;

        void add(const E &other) override;

        void add_all(const E *others, int count) override;

        void add_first(const E &other) override;

        void add_last(const E &other) override;

        void insert(int index, const E &other);

        E remove(int index) override;

        [[nodiscard]] int size() const override;

        [[nodiscard]] bool empty() const override;

        bool contains(const E &object) const override;

        void swap(int index1, int index2);

        E& operator[](int index) override;

        E& operator[](int index) const override;

    private:
        void require_valid_index(int index) const;

        bool need_expansion(int add_amount = 1);

        bool need_shrink();

        void expand();

        void shrink();

        void resize(int new_capacity);
};

#include "ArrayList.tpp"

#endif //ARRAYLIST_H
