#pragma once
#include <iterator>
#include <ranges>

#include "SinglyLinkedList.h"


template <typename T>
SinglyLinkedList<T>::SinglyLinkedList() {
    head(nullptr);
    size = 0;
}

template <typename T>
SinglyLinkedList<T>::~SinglyLinkedList() {//이거 파괴자는 순차적으로 돌아가면서 다 지워야하나?
    clear();
}

template<typename T>
void SinglyLinkedList<T>::add(T element) {
    Node newNode = new Node(element);
    if (isEmpty()) {
        head = newNode;
    }
    else {
        newNode->next = head;
    }
    ++size;
}

template<typename T>
void SinglyLinkedList<T>::insert(int index, T element) {
    if (index == 0) {
        add(element);
    }
    else {
        Node newNode = new Node(element);
        Node prevNode = head;

        for (int i = 0; i < index - 1; i++) {
            prevNode = prevNode->next;
        }
        newNode->next = prevNode->next;
        prevNode->next = newNode;
        ++size;
    }
}

template<typename T>
T SinglyLinkedList<T>::get(int index) const {
    if (index == 0) {
        return head->data;
    }

    Node prevNode = head;
    for (int i = 0; i< index - 1; i++) {
        prevNode = prevNode->next;
    }

    return prevNode->data;
}

template<typename T>
T SinglyLinkedList<T>::getHead() const {
    if (isEmpty()) {
        return nullptr;
    }
    return head->data;
}

template<typename T>
int SinglyLinkedList<T>::getSize() const {
    return size;
}

template<typename T>
bool SinglyLinkedList<T>::isEmpty() const {
    return size == 0;
}

template<typename T>
void SinglyLinkedList<T>::remove(int index) {
    if (index == 0) {
        removeFirst();
    }
    Node prevNode = head;
    for (int i = 0; i < index - 1; i++) {
        prevNode = prevNode->next;
    }

    prevNode->next = prevNode->next->next;

    delete prevNode->next;
    --size;
}

template<typename T>
void SinglyLinkedList<T>::remove(T element) {

}

template<typename T>
void SinglyLinkedList<T>::removeFirst() {
    head = head->next;
    delete head;
    --size;
}

template<typename T>
void SinglyLinkedList<T>::clear() {
    Node* currentNode = head;

    while (currentNode != nullptr) {
        Node* nextNode = currentNode->next;
        delete currentNode;
        currentNode = nextNode;
    }

    head = nullptr;
    size = 0;
}

