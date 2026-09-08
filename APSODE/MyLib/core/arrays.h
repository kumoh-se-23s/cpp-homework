//
// Created by leegu on 26. 9. 7..
//

#ifndef CPP_HOMEWORK_DYNAMIC_ARRAY_H
#define CPP_HOMEWORK_DYNAMIC_ARRAY_H

#include <collections.h>

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

            bool isValidIndex(int index) {
                return 0 <= index && index < this->size;
            }

            bool needExpansion(int addAmount = 1) {
                return addAmount + this->size >= this->capacity;
            }

            bool needCompression() {
                return this->size <= this->capacity * DEFAULT_COMPRESS_CONDITION;
            }

            int expandContainer() {
                return this->resize(this->capacity * DEFAULT_EXPAND_RATIO);
            }

            int compressContainer() {
                return this->resize(this->capacity * DEFAULT_COMPRESS_RATIO);
            }

            int resize(int newCapacity) {
                E *newContainer = new E[newCapacity];

                for (int containerIndex = 0; containerIndex < this->size; containerIndex++) {
                    newContainer[containerIndex] = this->container[containerIndex];
                }

                delete[] this->container;
                this->container = newContainer;
                this->capacity = newCapacity;

                return 0;
            }

        public:
            DynamicArray() {
                this->container = new E[DEFAULT_CAPACITY];
                this->capacity = DEFAULT_CAPACITY;
                this->size = 0;
            }

            DynamicArray(int manualCapacity) {
                this->container = new E[manualCapacity];
                this->capacity = manualCapacity;
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

            int getSize() const override {
                return this->size;
            }

            int getCapacity() const override {
                return this->capacity;
            }

            bool isEmpty() override {
                return this->size == 0;
            }

            bool isContains(const E &value) override {
                for (int containerIndex = 0; containerIndex < this->size; ++containerIndex) {
                    if (this->container[containerIndex] == value) {
                        return true;
                    }
                }

                return false;
            }

            int swap(int index1, int index2) {
                if (!this->isValidIndex(index1) || !this->isValidIndex(index2)) {
                    return 1;
                }

                E temp = this->container[index1];
                this->container[index1] = this->container[index2];
                this->container[index2] = temp;

                return 0;
            }

            int add(const E &value) override {
                if (this->needExpansion()) {
                    this->expandContainer();
                }

                this->container[this->size] = value;
                ++this->size;

                return 0;
            }

            int insert(int index, const E &value) override {
                if (!this->isValidIndex(index)) {
                    return 1;
                }

                if (index == this->size) {
                    return this->append(value);
                }

                if (this->needExpansion()) {
                    this->expandContainer();
                }

                for (int containerIndex = this->size - 1; containerIndex >= index; --containerIndex) {
                    this->container[containerIndex + 1] = this->container[containerIndex];
                }

                this->container[index] = value;

                return 0;
            }

            int removeAt(int index) override {
                if (!this->isValidIndex(index)) {
                    return 1;
                }

                for (int containerIndex = index; containerIndex < this->size - 1; containerIndex++) {
                    this->container[containerIndex] = this->container[containerIndex + 1];
                }

                --this->size;

                return 0;
            }
    };
}

#endif //CPP_HOMEWORK_DYNAMIC_ARRAY_H