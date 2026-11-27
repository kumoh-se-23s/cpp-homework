#pragma once
#include "Node.h"
#include "MyIterator.hpp"

template<typename T>
class MyLinkedList {
public:
    MyLinkedList();
    ~MyLinkedList();

    void addAsc(T* newItem);
    template <typename K> bool remove(const K& key);
    template <typename K> T* getByKey(const K& key) const;
    MyIterator<T> getIterator() const;
private:
    Node<T>* head;

    void addAfter(Node<T>* newNode, Node<T>* preNode);
    void removeAfter(Node<T>* preNode);
    void deleteAll(Node<T>* node);
};