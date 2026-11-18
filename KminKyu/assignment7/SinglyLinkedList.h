#ifndef SINGLYLINKEDLIST_H
#define SINGLYLINKEDLIST_H
#include "SingleNode.h"
#include "LinkedList.h"

template <typename T>
class SinglyLinkedList : public LinkedList<T>{
    SingleNode<T> *head = nullptr;
    int size = 0;
public:
    SinglyLinkedList() = default;

    SinglyLinkedList(SingleNode<T> head);

    ~SinglyLinkedList() override;

    SinglyLinkedList(const SinglyLinkedList &singlyLinkedList) = delete;

    SinglyLinkedList(SinglyLinkedList &&singlyLinkedList) noexcept;

    SinglyLinkedList &operator=(const SinglyLinkedList &otherLinkedList) = delete;

    SinglyLinkedList &operator=(SinglyLinkedList &&otherLinkedList);

    SingleNode<T> *getHead();

    int getSize();

    const SingleNode<T>& addFirst(T item) override;

    const SingleNode<T>& add(T item) override;

    SingleNode<T> & find(T item) override;

    bool remove(T) override;

    bool removeFirst(T) override;

    void clear();
private:

};

#include "SinglyLinkedList.tpp"
#endif // SINGLYLINKEDLIST_H