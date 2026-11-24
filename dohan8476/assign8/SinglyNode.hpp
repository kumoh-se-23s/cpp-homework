#pragma once
#include "Node.hpp"

template <typename T>
class SinglyNode : public Node<T> {
    SinglyNode* next;

    public:
        SinglyNode(T* data = nullptr, SinglyNode* next = nullptr)
            : Node<T>(data), next(next) {}

        SinglyNode* getNext() const { return next; }
        void setNext(SinglyNode* next) { this->next = next; }
};
