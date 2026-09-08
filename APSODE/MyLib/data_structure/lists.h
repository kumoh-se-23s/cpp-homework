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

            ArrayList(int manualCapacity) {
                this->container = new arrays::DynamicArray<E>(manualCapacity);
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

            int getSize() const override {
                return this->container->getSize();
            }

            int getCapacity() const override {
                return this->container->getCapacity();
            }

            int add(const E &value) override {
                return this->container->add(value);
            }

            int insert(int index, const E &value) override {
                return this->container->insert(index, value);
            }

            int removeAt(int index) override {
                return this->container->removeAt(index);
            }

            bool isEmpty() override {
                return this->container->isEmpty();
            }

            bool isContains(const E &value) override {
                return this->container->isContains(value);
            }

            E get(int index) override {
                if (!this->container->isValidIndex(index)) {
                    return nullptr;
                }

                return (*this->container)[index];
            }

            int set(int index, E &value) override {
                if (!this->container->isValidIndex(index)) {
                    return 1;
                }

                (*this->container)[index] = value;

                return 0;
            }

            int append(E &value) override {
                return this->container->add(value);
            }

            int addFirst(E &value) override {
                return this->container->insert(0, value);
            }

            int addLast(E &value) override {
                return this->append(value);
            }
    };
}

#endif //CPP_HOMEWORK_LISTS_H
