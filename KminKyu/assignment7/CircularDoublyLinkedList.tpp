#pragma once

template<typename T>
class CircularDoublyLinkedList;

template<typename T>
CircularDoublyLinkedList<T>::CircularDoublyLinkedList(DoubleNode<T> *head) {
    DoubleNode<T> *now = head;
    while (now->nextNode != nullptr && now->nextNode != head) {
        ++this->size;
    }
    if (now->nextNode == nullptr) {
        now->nextNode = head;
        head->prevNode = now;
    }
    this->head = head;
}


template<typename T>
CircularDoublyLinkedList<T>::CircularDoublyLinkedList(CircularDoublyLinkedList &&doublyLinkedList) noexcept {
    this->head = doublyLinkedList.head;
    this->size = doublyLinkedList.size;

    doublyLinkedList.head = nullptr;
    doublyLinkedList.size = 0;
}

template<typename T>
CircularDoublyLinkedList<T>::~CircularDoublyLinkedList() {
    clear();
}

template<typename T>
CircularDoublyLinkedList<T> & CircularDoublyLinkedList<T>::operator=(CircularDoublyLinkedList &&otherLinkedList)  noexcept {
    if (this == &otherLinkedList) {
        return *this;
    }
    clear();

    this->head = otherLinkedList.head;
    this->size = otherLinkedList.size;

    otherLinkedList.head = nullptr;
    otherLinkedList.size = 0;

    return *this;
}

template<typename T>
DoubleNode<T> *CircularDoublyLinkedList<T>::getHead() {
    return head;
}

template<typename T>
int CircularDoublyLinkedList<T>::getSize() {
    return size;
}

template<typename T>
const DoubleNode<T> *CircularDoublyLinkedList<T>::add(T item) {
    DoubleNode<T> *now = head;
    DoubleNode<T> *node = new DoubleNode<T>(item);

    //노드가 1개도 없는 경우
    if (size == 0) {
        head = node;
        node->nextNode = head;
        node->prevNode = head;
        return head;
    }

    while (now->nextNode != head && now->item <= item) {
        now = now->nextNode;
    }

    now->prevNode->nextNode = node;
    node->prevNode = now->prevNode;
    node->nextNode = now;
    now->prevNode = node;

    ++size;

    return node;
}

template<typename T>
DoubleNode<T> *CircularDoublyLinkedList<T>::find(T item) {
    if (size == 0) {
        return nullptr;
    }
    DoubleNode<T> *now = head;
    while (now->nextNode != head && now->item < item) {
        now = now->nextNode;
    }
    if (now->item == item) {
        return now;
    } else {
        return nullptr;
    }
}


template<typename T>
bool CircularDoublyLinkedList<T>::remove(T item) {
    if (size == 0) {
        return false;
    }
    DoubleNode<T> *now = head;
    while (now->nextNode != head && now->item != item) {
        now = now->nextNode;
    }
    if (now->item == item) {
        now->prevNode->nextNode = now->nextNode;
        now->nextNode->prevNode = now->prevNode;
        delete now;
        --size;
        return true;
    }
    return false;
}


template<typename T>
void CircularDoublyLinkedList<T>::clear() {
    DoubleNode<T> *now = head;
    while (now != nullptr) {
        DoubleNode<T> *temp = now;
        now = now->nextNode;
        delete temp;
        temp = nullptr;
    }
}
