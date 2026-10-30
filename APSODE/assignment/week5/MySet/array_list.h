//
// Created by leegu on 26. 10. 28..
//

#ifndef CPP_HOMEWORK_ARRAYS_H
#define CPP_HOMEWORK_ARRAYS_H

#include <stdexcept>

#include "collection.h"

template<typename E>
class ArrayList : public Collection<E> {
    constexpr static int DEFAULT_CAPACITY = 4;
    // 0.25 => 사용중인 공간이 전체 공간의 25% 일때, 압축
    constexpr static double DEFAULT_SHRINK_CONDITION = 0.25;
    constexpr static double DEFAULT_SHRINK_RATIO = 0.5;
    constexpr static int DEFAULT_EXPAND_RATIO = 2;

    E *container;
    int m_size;
    int capacity;

    public:
        ArrayList()
            : container(new E[DEFAULT_CAPACITY]), m_size(0), capacity(DEFAULT_CAPACITY) {
        }

        explicit ArrayList(int manual_capacity)
            : m_size(0), capacity(manual_capacity) {
            if (manual_capacity < 1) {
                throw std::invalid_argument("capacity must be positive");
            }

            this->container = new E[manual_capacity];
        }

        ArrayList(const E *elements, int count)
            : container(new E[DEFAULT_CAPACITY]), m_size(0), capacity(DEFAULT_CAPACITY) {
            this->ArrayList::add_all(elements, count);
        }

        ArrayList(const ArrayList &other)
            : container(new E[other.capacity]), m_size(other.m_size), capacity(other.capacity) {
            for (int index = 0; index < this->m_size; ++index) {
                this->container[index] = other.container[index];
            }
        }

        ArrayList &operator=(const ArrayList &other) {
            if (this == &other) {
                return *this;
            }

            E *new_container = new E[other.capacity];
            for (int index = 0; index < other.m_size; ++index) {
                new_container[index] = other.container[index];
            }

            delete[] this->container;
            this->container = new_container;
            this->m_size = other.m_size;
            this->capacity = other.capacity;
            return *this;
        }

        ~ArrayList() override {
            delete[] this->container;
        }

        E get(int index) const override {
            this->require_valid_index(index);

            return this->container[index];
        }

        void set(int index, const E &other) override {
            this->require_valid_index(index);

            this->container[index] = other;
        }

        void add(const E &other) override {
            if (this->need_expansion()) {
                this->expand();
            }

            this->container[this->m_size++] = other;
        }

        void add_all(const E *others, int count) override {
            if (count < 0) {
                throw std::invalid_argument("negative element count");
            }

            for (int index = 0; index < count; ++index) {
                this->add(others[index]);
            }
        }

        void add_first(const E &other) {
            this->insert(0, other);
        }

        void add_last(const E &other) {
            this->add(other);
        }

        void insert(int index, const E &other) {
            if (index < 0 || index > this->m_size) {
                throw std::out_of_range("index out of range");
            }

            if (this->need_expansion()) {
                this->expand();
            }

            for (int container_index = this->m_size; container_index > index; --container_index) {
                this->container[container_index] = this->container[container_index - 1];
            }

            this->container[index] = other;
            ++this->m_size;
        }

        E remove(int index) override {
            this->require_valid_index(index);

            E element = this->container[index];

            for (int container_index = index; container_index < this->m_size - 1; ++container_index) {
                this->container[container_index] = this->container[container_index + 1];
            }
            --this->m_size;

            if (this->need_shrink()) {
                this->shrink();
            }

            return element;
        }

        int size() const override {
            return this->m_size;
        }

        bool empty() const override {
            return this->m_size == 0;
        }

        bool contains(const E &object) const override {
            for (int index = 0; index < this->m_size; ++index) {
                if (this->container[index] == object) {
                    return true;
                }
            }

            return false;
        }

        void swap(int index1, int index2) {
            this->require_valid_index(index1);
            this->require_valid_index(index2);

            E temp = this->container[index1];
            this->container[index1] = this->container[index2];
            this->container[index2] = temp;
        }

        E &operator[](int index) {
            this->require_valid_index(index);
            return this->container[index];
        }

    private:
        void require_valid_index(int index) const {
            if (!(0 <= index && index < this->m_size)) {
                throw std::out_of_range("index out of range");
            }
        }

        bool need_expansion(int add_amount = 1) {
            return this->m_size + add_amount > this->capacity;
        }

        bool need_shrink() {
            return this->capacity > DEFAULT_CAPACITY
                   && this->m_size <= this->capacity * DEFAULT_SHRINK_CONDITION;
        }

        void expand() {
            this->resize(this->capacity * DEFAULT_EXPAND_RATIO);
        }

        void shrink() {
            int new_capacity = static_cast<int>(this->capacity * DEFAULT_SHRINK_RATIO);
            if (new_capacity < DEFAULT_CAPACITY) {
                new_capacity = DEFAULT_CAPACITY;
            }

            this->resize(new_capacity);
        }

        void resize(int new_capacity) {
            if (new_capacity < this->m_size) {
                throw std::invalid_argument("capacity smaller than size");
            }

            E *new_container = new E[new_capacity];
            for (int index = 0; index < this->m_size; ++index) {
                new_container[index] = this->container[index];
            }

            delete[] this->container;
            this->container = new_container;
            this->capacity = new_capacity;
        }
};

#endif //CPP_HOMEWORK_ARRAYS_H
