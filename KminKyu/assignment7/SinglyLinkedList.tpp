#pragma once

template<typename T>
class SinglyLinkedList;

template<typename T>
SinglyLinkedList<T>::SinglyLinkedList(SingleNode<T> *head) {

    //헤더를 nullptr 넣은 경우
    if (head == nullptr) {
        this->head = nullptr;
        this->size = 0;
        return;
    }

    SingleNode<T> *now = head;
    this-> size = 1;

    while (now->getNext() != nullptr) {
        now = now->getNext();
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
SinglyLinkedList<T> &SinglyLinkedList<T>::operator=(SinglyLinkedList &&otherLinkedList)  noexcept {
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
SingleNode<T> *SinglyLinkedList<T>::getHead() const {
    return head;
}

template<typename T>
int SinglyLinkedList<T>::getSize() const {
    return size;
}

template<typename T>
SingleNode<T> * SinglyLinkedList<T>::addFirst(SingleNode<T> *node) {
    node->setNext(head);
    this->head = node;
    ++size;
    return node;
}

template<typename T>
SingleNode<T> * SinglyLinkedList<T>::addNode(SingleNode<T> *node) {
    if (this->size == 0 || node->getItem() < head->getItem()) {
        return this->addFirst(node);
    }

    SingleNode<T> *now = head;

    while (now->getNext() != nullptr && now->getNext()->getItem() <= node->getItem()) {
        now = now->getNext();
    }

    node->setNext(now->getNext());
    now->setNext(node);
    ++size;

    return node;
}

template<typename T>
SingleNode<T> * SinglyLinkedList<T>::addAll(SingleNode<T> *node) {

    SingleNode<T> *now = node;
    while (now != nullptr) {
        SingleNode<T> *next = now->getNext();
        now->setNext(nullptr);
        addNode(now);
        now = next;
    }

    return node;
}
template<typename T>
SingleNode<T> *SinglyLinkedList<T>::add(SingleNode<T> *node) {
    if (node == nullptr) {
        return nullptr;
    }
    if (node->getNext() != nullptr) {
        return this->addAll(node);
    }
    return addNode(node);


}

template<typename T>
SingleNode<T> *SinglyLinkedList<T>::add(const T& item) {
    SingleNode<T> *node = new SingleNode<T>(item);
    return addNode(node);
}

template<typename T>
const SingleNode<T> *SinglyLinkedList<T>::find(const T& item) const {
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
SingleNode<T> * SinglyLinkedList<T>::find(const T &item) {
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
bool SinglyLinkedList<T>::removeFirst(const T& item) {
    if (head->getItem() != item) {
        return false;
    }
    SingleNode<T> *node = head;
    head = head->getNext();
    delete node;
    --size;
    return true;
}

template<typename T>
bool SinglyLinkedList<T>::remove(const T& item) {
    if (size == 0) {
        return false;
    } else if (size == 1 || head->getItem() >= item) {
        return removeFirst(item);
    }

    SingleNode<T> *deleteNode = head->getNext();
    SingleNode<T> *prev = head;
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
void SinglyLinkedList<T>::clear() {
    if (head == nullptr) {
        return;
    }
    SingleNode<T> *now = head;
    while (now != nullptr) {
        SingleNode<T> *deleteNode = now;
        now = now->getNext();
        delete deleteNode;
    }

    head = nullptr;
    size = 0;
}

template<typename T>
MyString SinglyLinkedList<T>::toString() const {
    MyString result = MyString();
    SingleNode<T> *now = head;

    while (now != nullptr) {
        result += now->toString();
        result += "\n";
        now = now->getNext();
    }
    return result;
}
template<typename T>
std::ostream & operator<<(std::ostream &out, const SinglyLinkedList<T> &singlyLinkedList) {
    SingleNode<T> *now = singlyLinkedList.getHead();

    while (now != nullptr) {
        out << now->getItem();
        now = now->getNext();
    }
    return out;
}


