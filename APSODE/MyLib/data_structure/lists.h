//
// Created by leegu on 26. 9. 8..
//

#ifndef CPP_HOMEWORK_LISTS_H
#define CPP_HOMEWORK_LISTS_H

#include "arrays.h"
#include "collections.h"



namespace nodes {
    template<typename T, typename N>
    class Node {
        protected:
            T item;
            N *nextNode;

        public:
            Node() {
                this->item = NULL;
                this->nextNode = nullptr;
            }

            Node(const T &item, N *next_node)
                : item(item), nextNode(next_node) {
            }

            virtual ~Node() {
                delete this->nextNode;
            }

            virtual const T &getItem() const {
                return this->item;
            }

            virtual void setItem(const T &newItem) {
                this->item = newItem;
            }

            virtual N *getNextNode() const {
                return this->nextNode;
            }

            virtual void setNextNode(N *newNextNode) {
                this->nextNode = newNextNode;
            }
    };

    template<typename T>
    class SingleConnectNode : public Node<T, SingleConnectNode<T> > {
        public:
            SingleConnectNode(const T &item, SingleConnectNode *next_node)
                : Node<T, SingleConnectNode>(item, next_node) {
            }
    };

    template<typename T>
    class DoubleConnectNode : public Node<T, DoubleConnectNode<T> > {
        protected:
            DoubleConnectNode *prevNode;

        public:
            DoubleConnectNode(const T &item, DoubleConnectNode *next_node, DoubleConnectNode *prev_node)
                : Node<T, DoubleConnectNode>(item, next_node), prevNode(prev_node) {
            }

            DoubleConnectNode *getPrevNode() const {
                return this->prevNode;
            }

            void setPrevNode(DoubleConnectNode *prev_node) {
                this->prevNode = prev_node;
            }
    };
}


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

            const E &get(int index) override {
                if (!this->container->isValidIndex(index)) {
                    return E();
                }

                return (*this->container)[index];
            }

            int set(int index, const E &value) override {
                if (!this->container->isValidIndex(index)) {
                    return 1;
                }

                (*this->container)[index] = value;

                return 0;
            }

            int append(const E &value) override {
                return this->container->add(value);
            }

            int addFirst(const E &value) override {
                return this->container->insert(0, value);
            }

            int addLast(const E &value) override {
                return this->append(value);
            }
    };

    template<typename E>
    class SinglyLinkedList : public collections::List<E> {
        private:
            nodes::SingleConnectNode<E> *head;

        public:
            SinglyLinkedList() {
                this->head = nullptr;
            }

            E *begin() override {
                return this->head;
            }

            E *end() override;

            const E *begin() const override;

            const E *end() const override;

            int getSize() const override;

            int getCapacity() const override;

            int add(const E &value) override;

            int insert(int index, const E &value) override;

            int removeAt(int index) override;

            bool isEmpty() override;

            bool isContains(const E &value) override;

            const E &get(int index) override;

            int set(int index, const E &value) override;

            int append(const E &value) override;

            int addFirst(const E &value) override;

            int addLast(const E &value) override;

            ~SinglyLinkedList() override;
    };
}

#endif //CPP_HOMEWORK_LISTS_H
