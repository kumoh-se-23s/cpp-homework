//
// Created by leegu on 26. 9. 7..
//

#ifndef CPP_HOMEWORK_DYNAMIC_ARRAY_H
#define CPP_HOMEWORK_DYNAMIC_ARRAY_H


namespace dynamic_array {
    constexpr int DEFAULT_CAPACITY = 16;

    template<typename E>
    class DynamicArray {
        private:
            E **container;
            int capacity;
            int size;

            bool isValidIndex(int index) {
                return 0 <= index && index < this->size;
            }

            void resize(int newCapacity) {
                E **newContainer = new E*[newCapacity];

                for (int containerIndex = 0; containerIndex < this->size; containerIndex++) {
                    newContainer[containerIndex] = this->container[containerIndex];
                }

                delete[] this->container;
                this->container = newContainer;
                this->capacity = newCapacity;
            }

        public:
            DynamicArray() {
                this->container = new E*[DEFAULT_CAPACITY];
                this->capacity = DEFAULT_CAPACITY;
                this->size = 0;
            }

            DynamicArray(int manual_size) {
                this->container = new E*[manual_size];
                this->capacity = manual_size;
                this->size = 0;
            }

            ~DynamicArray() {
                for (int containerIndex = 0; containerIndex < this->size; containerIndex++) {
                    delete this->container[containerIndex];
                }
                delete[] this->container;
            }

            int getCurrentSize() const {
                return this->size;
            }

            int getCurrentCapacity() const {
                return this->capacity;
            }

            int swap(int index1, int index2) {
                if (!this->isValidIndex(index1) || !this->isValidIndex(index2)) {
                    return 1;
                }

                E *temp = this->container[index1];
                this->container[index1] = this->container[index2];
                this->container[index2] = temp;

                return 0;
            }

            int add(const E &value) {
                if (this->size == this->capacity) {
                    this->resize(this->capacity == 0 ? DEFAULT_CAPACITY : this->capacity * 2);
                }

                this->container[this->size] = new E(value);
                ++this->size;

                return 0;
            }

            E& get(int index) {
                return *this->container[index];
            }

            int set(int index, const E &value) {
                if (!this->isValidIndex(index)) {
                    return 1;
                }

                *this->container[index] = value;

                return 0;
            }

            int removeAt(int index) {
                if (!this->isValidIndex(index)) {
                    return 1;
                }

                delete this->container[index];

                for (int containerIndex = index; containerIndex < this->size - 1; containerIndex++) {
                    this->container[containerIndex] = this->container[containerIndex + 1];
                }

                --this->size;

                return 0;
            }
    };
}

#endif //CPP_HOMEWORK_DYNAMIC_ARRAY_H