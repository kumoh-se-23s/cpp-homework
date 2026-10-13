//
// Created by leegu on 26. 9. 7..
//

#ifndef CPP_HOMEWORK_DYNAMIC_ARRAY_H
#define CPP_HOMEWORK_DYNAMIC_ARRAY_H

#include "collections.h"

namespace arrays {
    constexpr int DEFAULT_CAPACITY = 16;
    // 0.25 => 사용중인 공간이 전체 공간의 25% 일때, 압축
    constexpr float DEFAULT_COMPRESS_CONDITION = 0.25;


    constexpr float DEFAULT_COMPRESS_RATIO = 0.5;
    constexpr int DEFAULT_EXPAND_RATIO = 2;

    template<typename E>
    class DynamicArray : public collections::Collection<E>{
        private:
            E *container;
            int capacity;
            int size;

            bool need_expansion(int add_amount = 1) {
                return add_amount + this->size >= this->capacity;
            }

            bool need_compression() {
                return this->size <= this->capacity * DEFAULT_COMPRESS_CONDITION;
            }

            int expand_container() {
                return this->resize(this->capacity * DEFAULT_EXPAND_RATIO);
            }

            int compress_container() {
                int new_capacity = this->capacity * DEFAULT_COMPRESS_RATIO;

                if (new_capacity < DEFAULT_CAPACITY) {
                    new_capacity = DEFAULT_CAPACITY;
                }

                if (new_capacity == this->capacity) {
                    return 0;
                }

                return this->resize(new_capacity);
            }

            int resize(int new_capacity) {
                E *new_container = new E[new_capacity];

                for (int container_index = 0; container_index < this->size; container_index++) {
                    new_container[container_index] = this->container[container_index];
                }

                delete[] this->container;
                this->container = new_container;
                this->capacity = new_capacity;

                return 0;
            }

        public:
            DynamicArray() {
                this->container = new E[DEFAULT_CAPACITY];
                this->capacity = DEFAULT_CAPACITY;
                this->size = 0;
            }

            DynamicArray(int manual_capacity) {
                this->container = new E[manual_capacity];
                this->capacity = manual_capacity;
                this->size = 0;
            }

            ~DynamicArray() override {
                delete[] this->container;
            }

            E* begin() override {
                return this->container;
            }

            E* end() override {
                return this->container + this->size;
            }

            const E* begin() const override {
                return this->container;
            }

            const E* end() const override {
                return this->container + this->size;
            }

            int get_size() const override {
                return this->size;
            }

            int get_capacity() const override {
                return this->capacity;
            }

            bool is_empty() override {
                return this->size == 0;
            }

            bool is_valid_index(int index) {
                return 0 <= index && index < this->size;
            }

            bool is_contains(const E &value) override {
                for (int container_index = 0; container_index < this->size; ++container_index) {
                    if (this->container[container_index] == value) {
                        return true;
                    }
                }

                return false;
            }

            int swap(int index1, int index2) {
                if (!this->is_valid_index(index1) || !this->is_valid_index(index2)) {
                    return 1;
                }

                E temp = this->container[index1];
                this->container[index1] = this->container[index2];
                this->container[index2] = temp;

                return 0;
            }

            int add(const E &value) override {
                if (this->need_expansion()) {
                    this->expand_container();
                }

                this->container[this->size] = value;
                ++this->size;

                return 0;
            }

            int insert(int index, const E &value) override {
                if (!this->is_valid_index(index)) {
                    if (index == this->size) {
                        return this->add(value);
                    }

                    return 1;
                }

                if (this->need_expansion()) {
                    this->expand_container();
                }

                for (int container_index = this->size - 1; container_index >= index; --container_index) {
                    this->container[container_index + 1] = this->container[container_index];
                }

                this->container[index] = value;

                return 0;
            }

            int remove_at(int index) override {
                if (!this->is_valid_index(index)) {
                    return 1;
                }

                if (this->need_compression()) {
                    this->compress_container();
                }

                for (int container_index = index; container_index < this->size - 1; container_index++) {
                    this->container[container_index] = this->container[container_index + 1];
                }

                --this->size;

                return 0;
            }

            E& operator[](int index) {
                return this->container[index];
            }
    };
}

#endif //CPP_HOMEWORK_DYNAMIC_ARRAY_H
