#pragma once

#include "SinglyNode.hpp"

template <typename T>
class SinglyLinkedList {
    public:
        SinglyLinkedList() : head(nullptr), size(0) {}
        ~SinglyLinkedList() { clear(); }

        void add(T* newNode);
        void insert(int index, T* newNode);
        void insertAscend(T* newNode);

        template <typename KeyType>
        T* remove(const KeyType& element);
        T* remove(int index);
        T* removeFirst();

        int getSize() const;
        bool isEmpty() const;
        T* get(int index) const;
        SinglyNode<T>* getHead() const;
        void clear();
        template <typename KeyType>
        T* find(const KeyType& element) const;

        void moveFrom(SinglyLinkedList& other);

    private:
        SinglyNode<T>* head;
        int size;
};

template<typename T>
void SinglyLinkedList<T>::add(T* newNode) {
    if (newNode == nullptr) return;

    head = new SinglyNode<T>(newNode, head);

    ++size;
}

template<typename T>
void SinglyLinkedList<T>::insert(int index, T* newNode) {
    if (index < 0 || index > size || newNode == nullptr) return;

    if (index == 0) {
        add(newNode);
        return;
    }

    SinglyNode<T>* prev = head;
    for (int i = 0; i < index - 1; ++i) {
        prev = prev->getNext();
    }
    prev->setNext(new SinglyNode<T>(newNode, prev->getNext()));
    ++size;
}

template<typename T>
void SinglyLinkedList<T>::insertAscend(T* newNode) {
    if (newNode == nullptr) return;

    if (isEmpty() || *newNode < *(head->getData())) {
        add(newNode);
        return;
    }

    SinglyNode<T>* curr = head;

    while (curr->getNext() != nullptr && *(curr->getNext()->getData()) < *newNode) {
        curr = curr->getNext();
    }

    curr->setNext(new SinglyNode<T>(newNode, curr->getNext()));
    ++size;
}

template<typename T>
T* SinglyLinkedList<T>::get(int index) const {
    if (index < 0 || index >= size) return nullptr;

    SinglyNode<T>* curr = head;

    for (int i = 0; i < index; ++i) {
        curr = curr->getNext();
    }

    return curr->getData();
}

template<typename T>
SinglyNode<T>* SinglyLinkedList<T>::getHead() const {
    return head;
}

template<typename T>
int SinglyLinkedList<T>::getSize() const {
    return size;
}

template<typename T>
bool SinglyLinkedList<T>::isEmpty() const {
    return size == 0;
}

//이번엔 다 MyString이라 int면 index 호출
template<typename T>
T* SinglyLinkedList<T>::remove(const int index) {
    if (index < 0 || index >= size) return nullptr;
    if (index == 0) return removeFirst();

    SinglyNode<T>* prev = head;
    for (int i = 0; i < index - 1; ++i) {
        prev = prev->getNext();
    }

    SinglyNode<T>* target = prev->getNext();
    prev->setNext(target->getNext());

    T* data = target->getData();
    target->setData(nullptr);
    delete target;

    --size;
    return data;
}

template <typename T> template <typename KeyType>
T* SinglyLinkedList<T>::remove(const KeyType& element) {
    if (head == nullptr) return nullptr;
    if (*(head->getData()) == element) return removeFirst();

    SinglyNode<T>* curr = head;
    while (curr->getNext() != nullptr && !(*(curr->getNext()->getData()) == element)) {
        curr = curr->getNext();
    }
    if (curr->getNext() == nullptr) return nullptr;

    SinglyNode<T>* target = curr->getNext();
    curr->setNext(target->getNext());

    T* data = target->getData();
    target->setData(nullptr);
    delete target;

    --size;
    return data;
}


template<typename T>
T* SinglyLinkedList<T>::removeFirst() {
    if (head == nullptr) return nullptr;

    SinglyNode<T>* target = head;
    head = head->getNext();

    T* data = target->getData();
    target->setData(nullptr);
    delete target;

    --size;
    return data;
}

template<typename T>
void SinglyLinkedList<T>::clear() {
    while (!isEmpty()) {
        delete removeFirst();
    }
}

template <typename T>
template <typename KeyType>
T* SinglyLinkedList<T>::find(const KeyType& element) const {
    SinglyNode<T>* curr = head;
    while (curr != nullptr) {
        if (*(curr->getData()) == element) {
            return curr->getData();
        }
        curr = curr->getNext();
    }
    return nullptr;
}

//혹시 모르니 남겨둠
//테이블에서 bucket을 resize했을때 헤드만 옮겨서 동작가능
template<typename T>
void SinglyLinkedList<T>::moveFrom(SinglyLinkedList& other) {
    this->head = other.head;
    this->size = other.size;
    other.head = nullptr;
    other.size = 0;
}