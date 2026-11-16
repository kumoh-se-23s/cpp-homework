#pragma once

#include <iterator>

#include "SinglyLinkedList.h"


template <typename T>
SinglyLinkedList<T>::SinglyLinkedList() : head(nullptr), size(0) {}

template <typename T>
SinglyLinkedList<T>::~SinglyLinkedList() {
    // double free 위험으로 구현X
    // delete는 상위 계층에서 관리
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
    if (outOfRange(index) || newNode == nullptr) return;

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

    // head나 newNode가 주소값 가지고 있으니까 역참조로 값 가져오기
    if (isEmpty() || *newNode < *head) {
        newNode->next = head;
        head = newNode;
    } else {
        T* currentNode = head;

        //얘도 마찬가지로 역참조
        while (currentNode->next != nullptr && *(currentNode->next) < *newNode) {
            currentNode = currentNode->next;
        }
        newNode->next = currentNode->next;
        currentNode->next = newNode;
    }
    ++size;
}

template<typename T>
T* SinglyLinkedList<T>::get(int index) const {
    if (outOfRange(index) || isEmpty()) {
        return nullptr;
    }

    T* curr = head;
    for (int i = 0; i< index - 1; i++) {
        curr = curr->next;
    }

    return curr;
}

template<typename T>
T* SinglyLinkedList<T>::getHead() const {
    if (isEmpty()) {
        return nullptr;
    }
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

template<typename T>
T* SinglyLinkedList<T>::remove(const int index) {
    if (outOfRange(index) || head == nullptr) return nullptr;

    if (index == 0) {
        return removeFirst();
    }
    T* target = head;
    for (int i = 0; i < index - 1; i++) {
        target = target->next;
    }

    target->next = target->next->next;;
    --size;

    return target;
}

template <typename T> template <typename KeyType>
T* SinglyLinkedList<T>::remove(const KeyType& element) {
    if (head == nullptr) return nullptr;

    // 맨 앞(head)인 경우
    if (*head == element) {
        return removeFirst();
    }

    // 중간이나 끝
    T* curr = head;
    while (curr->next != nullptr && !(*(curr->next) == element)) {
        curr = curr->next;
    }

    if (curr->next == nullptr) return nullptr; // 끝까지 갔는데 못 찾음

    // 삭제
    T* target = curr->next;
    curr->next = target->next;
    target->next = nullptr;
    --size;

    return target;
}


template<typename T>
T* SinglyLinkedList<T>::removeFirst() {
    if (head == nullptr) return nullptr;

    T* target = head;
    head = head->next;
    target->next = nullptr;
    --size;

    return target;
}

template<typename T>
bool SinglyLinkedList<T>::outOfRange(int index) const {
    return index < 0 || index >= size;
}


