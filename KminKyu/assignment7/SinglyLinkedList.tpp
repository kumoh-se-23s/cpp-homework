#pragma once

template<typename T>
class SinglyLinkedList;

template<typename T>
SinglyLinkedList<T>::SinglyLinkedList(SingleNode<T> head) {
    SingleNode<T> now = head;
    while (now.nextNode != nullptr) {
        ++this->size;
    }
    this->head = head;
}


template<typename T>
SinglyLinkedList<T>::SinglyLinkedList(SinglyLinkedList &&singlyLinkedList) noexcept {
    this->head = singlyLinkedList.head;
    this->size = singlyLinkedList.size;

    singlyLinkedList.head = nullptr;
    singlyLinkedList.size = 0;
}

template<typename T>
SinglyLinkedList<T>::~SinglyLinkedList() {
    clear();
}

template<typename T>
SinglyLinkedList<T> & SinglyLinkedList<T>::operator=(SinglyLinkedList &&otherLinkedList) {
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
SingleNode<T> *SinglyLinkedList<T>::getHead() {
    return head;
}

template<typename T>
int SinglyLinkedList<T>::getSize() {
    return size;
}

template<typename T>
const SingleNode<T>& SinglyLinkedList<T>::addFirst(T item) {
    SingleNode<T> node = new SingleNode<T>(item);
    node.setNext(head);
    this->head = node;
    ++size;
    return node;
}

template<typename T>
const SingleNode<T>& SinglyLinkedList<T>::add(T item) {
    return head;
}

template<typename T>
SingleNode<T> &SinglyLinkedList<T>::find(T item) {
    return head;
}

template<typename T>
bool SinglyLinkedList<T>::remove(T) {
    return false;
}

template<typename T>
bool SinglyLinkedList<T>::removeFirst(T) {
    return false;
}

template<typename T>
void SinglyLinkedList<T>::clear() {
    SingleNode<T> *now = static_cast<SingleNode<T>>(this->head);
    while (now != nullptr) {
        // Node<T> next = now->nextNode;
    }
    return;
}
