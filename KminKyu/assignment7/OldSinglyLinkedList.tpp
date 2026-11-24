#pragma once

template<typename T>
class OldSinglyLinkedList;

template<typename T>
OldSinglyLinkedList<T>::OldSinglyLinkedList(OldSingleNode<T> *head) {

    //헤더를 nullptr 넣은 경우
    if (head == nullptr) {
        this->head = nullptr;
        this->size = 0;
        return;
    }

    OldSingleNode<T> *now = head;
    this-> size = 1;

    while (now->getNext() != nullptr) {
        now = now->getNext();
        ++this->size;
    }
    this->head = head;
}


template<typename T>
OldSinglyLinkedList<T>::OldSinglyLinkedList(OldSinglyLinkedList &&singlyLinkedList) noexcept {
    this->head = singlyLinkedList.head;
    this->size = singlyLinkedList.size;

    singlyLinkedList.head = nullptr;
    singlyLinkedList.size = 0;
}

template<typename T>
OldSinglyLinkedList<T>::~OldSinglyLinkedList() {
    clear();
}

template<typename T>
OldSinglyLinkedList<T> &OldSinglyLinkedList<T>::operator=(OldSinglyLinkedList &&otherLinkedList)  noexcept {
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
OldSingleNode<T> *OldSinglyLinkedList<T>::getHead() const {
    return head;
}

template<typename T>
int OldSinglyLinkedList<T>::getSize() const {
    return size;
}

template<typename T>
OldSingleNode<T> * OldSinglyLinkedList<T>::addFirst(OldSingleNode<T> *node) {
    node->setNext(head);
    this->head = node;
    ++size;
    return node;
}

template<typename T>
OldSingleNode<T> * OldSinglyLinkedList<T>::addNode(OldSingleNode<T> *node) {
    if (this->size == 0 || node->getItem() < head->getItem()) {
        return this->addFirst(node);
    }

    OldSingleNode<T> *now = head;

    while (now->getNext() != nullptr && now->getNext()->getItem() <= node->getItem()) {
        now = now->getNext();
    }

    node->setNext(now->getNext());
    now->setNext(node);
    ++size;

    return node;
}

template<typename T>
OldSingleNode<T> * OldSinglyLinkedList<T>::addAll(OldSingleNode<T> *node) {

    OldSingleNode<T> *now = node;
    while (now != nullptr) {
        OldSingleNode<T> *next = now->getNext();
        now->setNext(nullptr);
        addNode(now);
        now = next;
    }

    return node;
}
template<typename T>
OldSingleNode<T> *OldSinglyLinkedList<T>::add(OldSingleNode<T> *node) {
    if (node == nullptr) {
        return nullptr;
    }
    if (node->getNext() != nullptr) {
        return this->addAll(node);
    }
    return addNode(node);


}

template<typename T>
OldSingleNode<T> *OldSinglyLinkedList<T>::add(const T& item) {
    OldSingleNode<T> *node = new OldSingleNode<T>(item);
    return addNode(node);
}

template<typename T>
const OldSingleNode<T> *OldSinglyLinkedList<T>::find(const T& item) const {
    if (size == 0) {
        return nullptr;
    }
    OldSingleNode<T> *now = head;
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
OldSingleNode<T> * OldSinglyLinkedList<T>::find(const T &item) {
    if (size == 0) {
        return nullptr;
    }
    OldSingleNode<T> *now = head;
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
bool OldSinglyLinkedList<T>::removeFirst(const T& item) {
    if (head->getItem() != item) {
        return false;
    }
    OldSingleNode<T> *node = head;
    head = head->getNext();
    delete node;
    --size;
    return true;
}

template<typename T>
bool OldSinglyLinkedList<T>::remove(const T& item) {
    if (size == 0) {
        return false;
    } else if (size == 1 || head->getItem() >= item) {
        return removeFirst(item);
    }

    OldSingleNode<T> *deleteNode = head->getNext();
    OldSingleNode<T> *prev = head;
    while (deleteNode->getNext() != nullptr && deleteNode->getItem() < item) {
        prev = deleteNode;
        deleteNode = deleteNode->getNext();
    }
    if (deleteNode == nullptr || deleteNode->getItem() != item) {
        return false;
    }
    prev->setNext(deleteNode->getNext());

    delete deleteNode;
    --size;
    return true;
}

template<typename T>
void OldSinglyLinkedList<T>::clear() {
    if (head == nullptr) {
        return;
    }
    OldSingleNode<T> *now = head;
    while (now != nullptr) {
        OldSingleNode<T> *deleteNode = now;
        now = now->getNext();
        delete deleteNode;
    }

    head = nullptr;
    size = 0;
}

template<typename T>
OOldMyString OldSinglyLinkedList<T>::toString() const {
    OOldMyString result = OOldMyString();
    OldSingleNode<T> *now = head;

    while (now != nullptr) {
        result += now->toString();
        result += "\n";
        now = now->getNext();
    }
    return result;
}
template<typename T>
std::ostream & operator<<(std::ostream &out, const OldSinglyLinkedList<T> &singlyLinkedList) {
    OldSingleNode<T> *now = singlyLinkedList.getHead();

    while (now != nullptr) {
        out << now->getItem();
        now = now->getNext();
    }
    return out;
}


