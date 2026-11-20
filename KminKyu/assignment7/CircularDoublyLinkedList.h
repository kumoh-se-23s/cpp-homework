#ifndef CIRCULARDOUBLYLINKEDLIST_H
#define CIRCULARDOUBLYLINKEDLIST_H
#include "DoubleNode.h"

template <typename T>
class CircularDoublyLinkedList {
    DoubleNode<T> *head = nullptr;
    int size = 0;
public:
    CircularDoublyLinkedList() = default;

    CircularDoublyLinkedList(DoubleNode<T> *head);

    ~CircularDoublyLinkedList();

    CircularDoublyLinkedList(const CircularDoublyLinkedList &singlyLinkedList) = delete;

    CircularDoublyLinkedList(CircularDoublyLinkedList &&singlyLinkedList) noexcept;

    CircularDoublyLinkedList &operator=(const CircularDoublyLinkedList &otherLinkedList) = delete;

    CircularDoublyLinkedList &operator=(CircularDoublyLinkedList &&otherLinkedList) noexcept;

    DoubleNode<T> *getHead() const;

    int getSize() const;

    DoubleNode<T> *add(DoubleNode<T> *node);

    DoubleNode<T> *add(const T& item);

    const DoubleNode<T> *find(const T& item) const;

    DoubleNode<T> *find(const T& item);

    bool remove(const T& item);

    void clear();

    MyString toString() const;

private:
    //정렬되지 않은 여러개의 노드가 들어오는것을 가정
    DoubleNode<T> *addAll(DoubleNode<T> *node);

    DoubleNode<T> *addNode(DoubleNode<T> *node);

};
template<typename T>
std::ostream &operator<<(std::ostream &out, const CircularDoublyLinkedList<T> &doublyLinkedList);
#include "CircularDoublyLinkedList.tpp"
#endif // CIRCULARDOUBLYLINKEDLIST_H
