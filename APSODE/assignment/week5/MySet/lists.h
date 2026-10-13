//
// Created by leegu on 26. 9. 8..
//

#ifndef CPP_HOMEWORK_LISTS_H
#define CPP_HOMEWORK_LISTS_H

#include "arrays.h"
#include "collections.h"


namespace lists {
    template<typename E>
    class ArrayList : public collections::List<E> {
        private:
            arrays::DynamicArray<E> *container;

        public:
            ArrayList() {
                this->container = new arrays::DynamicArray<E>();
            }

            ArrayList(int manual_capacity) {
                this->container = new arrays::DynamicArray<E>(manual_capacity);
            }

            ~ArrayList() {
                delete this->container;
            }

            E *begin() override {
                return this->container->begin();
            }

            E *end() override {
                return this->container->end();
            }

            const E *begin() const override {
                return this->container->begin();
            }

            const E *end() const override {
                return this->container->end();
            }

            int get_size() const override {
                return this->container->get_size();
            }

            int get_capacity() const override {
                return this->container->get_capacity();
            }

            int add(const E &value) override {
                return this->container->add(value);
            }

            int insert(int index, const E &value) override {
                return this->container->insert(index, value);
            }

            int remove_at(int index) override {
                return this->container->remove_at(index);
            }

            bool is_empty() override {
                return this->container->is_empty();
            }

            bool is_contains(const E &value) override {
                return this->container->is_contains(value);
            }

            const E &get(int index) override {
                if (!this->container->is_valid_index(index)) {
                    return E();
                }

                return (*this->container)[index];
            }

            int set(int index, const E &value) override {
                if (!this->container->is_valid_index(index)) {
                    return 1;
                }

                (*this->container)[index] = value;

                return 0;
            }

            int append(const E &value) override {
                return this->container->add(value);
            }

            int add_first(const E &value) override {
                return this->container->insert(0, value);
            }

            int add_last(const E &value) override {
                return this->append(value);
            }
    };
}

#endif //CPP_HOMEWORK_LISTS_H
