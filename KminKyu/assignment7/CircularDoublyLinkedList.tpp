#pragma once

template<typename T>
class CircularDoublyLinkedList;

template<typename T>
CircularDoublyLinkedList<T>::CircularDoublyLinkedList(DoubleNode<T> *head) {
    //헤드가 비어있는 경우
    if (head == nullptr) {
        this->head = nullptr;
        this->size = 0;
        return;
    }

    DoubleNode<T> *now = head;
    this->size = 1;

    while (now->getNext() != nullptr && now->getNext() != head) {
        ++size;
        now = now->getNext();

    }
    if (now->getNext() == nullptr) {
        now->setNext(head);
        head->setPrev(now);
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
CircularDoublyLinkedList<T> & CircularDoublyLinkedList<T>::operator=(CircularDoublyLinkedList &&otherLinkedList) noexcept {
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
DoubleNode<T> *CircularDoublyLinkedList<T>::getHead() const {
    return head;
}

template<typename T>
int CircularDoublyLinkedList<T>::getSize() const {
    return size;
}

template<typename T>
DoubleNode<T> *CircularDoublyLinkedList<T>::add(DoubleNode<T> *node) {
    if (node == nullptr) {
        return nullptr;
    }

    if ((node->getNext() != nullptr && node->getNext() != node) || (node->getPrev() != nullptr && node->getPrev() != node)) {
        return this->addAll(node);
    } else {
        return addNode(node);
    }
}

template<typename T>
DoubleNode<T> *CircularDoublyLinkedList<T>::add(const T& item) {

    DoubleNode<T> *node = new DoubleNode<T>(item);
    return addNode(node);
}

template<typename T>
DoubleNode<T> *CircularDoublyLinkedList<T>::addAll(DoubleNode<T> *node) {

    DoubleNode<T> *now = node;
    while (now->getPrev() != nullptr && now->getPrev() != node) {
        now = now->getPrev();
    }
    if (now->getPrev() != nullptr) {
        now->getPrev()->setNext(nullptr);
    }

    while (now != nullptr) {
        DoubleNode<T> *next = now->getNext();
        now->setPrev(nullptr);
        now->setNext(nullptr);
        addNode(now);
        now = next;
    }

    return node;
}

template<typename T>
DoubleNode<T> *CircularDoublyLinkedList<T>::addNode(DoubleNode<T> *node) {
    DoubleNode<T> *now = head;

    //노드가 1개도 없는 경우
    if (size == 0) {
        head = node;
        node->setNext(head);
        node->setPrev(head);
        ++size;
        return head;
    }

    do {
        if (now->getItem() > node->getItem()) {
            break;
        }
        now = now->getNext();

    } while (now != head);

    node->setPrev(now->getPrev());
    node->setNext(now);
    now->getPrev()->setNext(node);
    now->setPrev(node);

    //헤드 앞에 추가한 경우
    if (node->getItem() < head->getItem()) {
        head = node;
    }

    ++size;

    return node;

}
template<typename T>
const DoubleNode<T> *CircularDoublyLinkedList<T>::find(const T &item) const {
    if (size == 0) {
        return nullptr;
    }
    DoubleNode<T> *now = head;
    do {
        if (now->getItem() >= item) {
            break;
        }
        now = now->getNext();

    } while (now != head);

    if (now->getItem() == item) {
        return now;
    } else {
        return nullptr;
    }
}

template<typename T>
DoubleNode<T> *CircularDoublyLinkedList<T>::find(const T &item) {
    if (size == 0) {
        return nullptr;
    }
    DoubleNode<T> *now = head;
    do {
        if (now->getItem() >= item) {
            break;
        }
        now = now->getNext();

    } while (now != head);

    if (now->getItem() == item) {
        return now;
    } else {
        return nullptr;
    }
}


template<typename T>
bool CircularDoublyLinkedList<T>::remove(const T &item) {
    if (size == 0) {
        return false;
    }
    DoubleNode<T> *now = head;
    do {
        if (now->getItem() == item) {
            break;
        }
        now = now->getNext();

    } while (now != head);

    if (now->getItem() != item) {
        return false;
    }
    now->getPrev()->setNext(now->getNext());
    now->getNext()->setPrev(now->getPrev());

    if (size == 1) {
        head = nullptr;
    } else if (now == head) {
        head = now->getNext();
    }

    delete now;
    --size;
    return true;
}


template<typename T>
void CircularDoublyLinkedList<T>::clear() {
    if (head == nullptr) {
        return;
    }

    DoubleNode<T> *now = head->getNext();
    while (now != head) {
        DoubleNode<T> *deleteNode = now;
        now = now->getNext();
        delete deleteNode;
    }
    delete head;
    head = nullptr;
    size = 0;
}

template<typename T>
MyString CircularDoublyLinkedList<T>::toString() const {
    MyString result = MyString();
    if (size == 0) {
        return result;
    }
    DoubleNode<T> *now = head;

    do {
        result += now->toString();
        result += "\n";
        now = now->getNext();
    } while (now != head);
    return result;
}
template<typename T>
std::ostream & operator<<(std::ostream &out, const CircularDoublyLinkedList<T> &doublyLinkedList) {
    if (doublyLinkedList.getSize() == 0) {
        return out;
    }
    DoubleNode<T> *now = doublyLinkedList.getHead();
    do {
        out << now->getItem();
        out << "\n";
        now = now->getNext();
    } while(now != doublyLinkedList.getHead());
    return out;
}

