//
// Created by KminKyu on 2026-09-07.
//

#ifndef CPP_HOMEWORK_GIT_MYARRAYLIST_H
#define CPP_HOMEWORK_GIT_MYARRAYLIST_H

namespace dynamic_array {
    template <typename T>
    class MyArrayList {
        const int DEFAULT_ARRAY_SIZE = 2;
    private:
        T* items;
        int size;
        int capacity;

    public:
        MyArrayList() {
            this->items = new T[DEFAULT_ARRAY_SIZE];
            this->capacity = DEFAULT_ARRAY_SIZE;
            this->size = 0;
        };

        ~MyArrayList() {
            delete[] this->items;
        }

        int getSize() const {
            return this->size;
        }

        int getCapacity() const {
            return this->capacity;
        }

        void resize(int resize_capacity) {
            T* temp = new T[resize_capacity];
            for (int i = 0; i < this->capacity; i++) {
                temp[i] = this->items[i];
            }
            this->capacity = resize_capacity;
            this->items = temp;
        }
        void append(const T &item) {
            if (this->size == this->capacity) {
                this->resize(this->capacity * 2);
            }
            this->items[this->size] = item;
            ++this->size;
        }
        void clear() {
            this->size = 0;
            delete[] this->items;
        }

        T get(int index) const {
            if (index < this->size) {
                return this->items[index];
            } else {
                return nullptr;
            }
        }

        bool isEmpty() {
            return this->size == 0;
        }
        T pop() const {
            if (!this->isEmpty()) {
                return this->items[this->size--];
            } else {
                return nullptr;
            }
        }

    };
}

#endif //CPP_HOMEWORK_GIT_MYARRAYLIST_H

