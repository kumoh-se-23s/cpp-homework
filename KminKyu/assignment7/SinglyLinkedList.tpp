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
SinglyLinkedList<T> & SinglyLinkedList<T>::operator=(SinglyLinkedList &&otherLinkedList)  noexcept {
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
const SingleNode<T> *SinglyLinkedList<T>::addFirst(T item) {
    SingleNode<T> *node = new SingleNode<T>(item);
    node->setNext(head);
    this->head = node;
    ++size;
    return node;
}

template<typename T>
const SingleNode<T> *SinglyLinkedList<T>::add(T item) {
    if (size == 0 || item < head->getItem()) {
        return addFirst(item);
    }
    SingleNode<T> *now = head;

    while (now->getNext() != nullptr && now->getNext()->getItem() <= item) {
        now = now->getNext();
    }

    SingleNode<T> *node = new SingleNode<T>(item);
    node->setNext(now->getNext());
    ++size;
    now->setNext(node);
    return node;
}

template<typename T>
SingleNode<T> *SinglyLinkedList<T>::find(T item) {
    if (size == 0) {
        return nullptr;
    }
    SingleNode<T> *now = head;
    while (now->getNext() != nullptr && now->getItem() < item) {
        now = now->getNext();
    }
    if (now->getItem() == item) {
        return now;
    } else {
        return nullptr;
    }
}
template<typename T>
bool SinglyLinkedList<T>::removeFirst(T item) {
    if (size == 0 || head->getItem() != item) {
        return false;
    }
    SingleNode<T>* node = head;
    head = head->getNext();
    delete node;
    head = nullptr;
    --size;
    return true;
}

template<typename T>
bool SinglyLinkedList<T>::remove(T item) {
    if (size == 0) {
        return false;
    } else if (size == 1 || head->getItem() == item) {
        return removeFirst(item);
    }

    SingleNode<T> *now = head;
    while (now->getNext() != nullptr && now->getNext()->getItem() != item) {
        now = now->getNext();
    }
    if (now->getNext()->getItem() == item) {
        SingleNode<T> *temp = now->getNext();
        now->getNext() = temp->getNext();
        delete temp;
        temp = nullptr;
        --size;

        return true;
    }
    return false;
}



template<typename T>
void SinglyLinkedList<T>::clear() {
    SingleNode<T> *now = head;
    while (now != nullptr) {
        SingleNode<T> *temp = now;
        now = now->getNext();
        delete temp;
        temp = nullptr;
    }
}
