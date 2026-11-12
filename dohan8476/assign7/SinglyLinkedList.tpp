#pragma once

#include "SinglyLinkedList.h"


template <typename T>
SinglyLinkedList<T>::SinglyLinkedList() {
    head(nullptr);
    size = 0;
}

template <typename T>
SinglyLinkedList<T>::~SinglyLinkedList() {
    clear();
}

template<typename T>
void SinglyLinkedList<T>::add(T* newNode) {
    if (newNode == nullptr) return;

    newNode->next = head;
    head = newNode;

    ++size;
}

template<typename T>
void SinglyLinkedList<T>::insert(int index, T* newNode) {
    if (newNode == nullptr || index < 0 || index > size) return;

    if (index == 0) {
        add(newNode);
    } else {
        T* prevNode = head;
        for (int i = 0; i < index - 1; i++) {
            prevNode = prevNode->next;
        }
        newNode->next = prevNode->next;
        prevNode->next = newNode;
        ++size;
    }
}

template<typename T>
void SinglyLinkedList<T>::insertAscend(T* newNode) {
    if (newNode == nullptr) return;
    newNode->next = nullptr;

    if (isEmpty() || *newNode < *head) {
        newNode->next = head;
        head = newNode;
    } else {
        T* currentNode = head;

        while (currentNode->next != nullptr && *(currentNode->next) < *newNode) {
            currentNode = currentNode->next;
        }
        newNode->next = currentNode->next;
        currentNode->next = newNode;
    }
    ++size;
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

template <typename T> template <typename KeyType>
T SinglyLinkedList<T>::remove(const KeyType& key) {
    if (head == nullptr) return nullptr;

    if (*head == key) {
        T* targetNode = head;
        head = head->next;
        targetNode->next = nullptr;
    }
    else {
        T curr = head;
        while (curr->next != nullptr && !(*(curr->next) == key)) {
            curr = curr->next;
        }

        if (curr->next == nullptr) return nullptr;

        T targetNode = curr->next;
        curr->next = targetNode->next;
        targetNode->next = nullptr;
    }
    return targetNode;
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

