#pragma once
#include "MyLinkedList.h"

template<typename T>
MyLinkedList<T>::MyLinkedList() {
    Node<T>* dummy = new Node<T>(nullptr);
    head = dummy;
}

template<typename T>
MyLinkedList<T>::~MyLinkedList() {
    deleteAll(head);
}

//오름차순으로 추가
template<typename T>
void MyLinkedList<T>::addAsc(T* newItem) {
    Node<T>* newNode = new Node<T>(newItem);

    bool isInserted = false;
    Node<T>* preNode = head;
    for (Node<T> curNode = head->getNextNode(); !isInserted && curNode != nullptr; curNode = curNode->getNextNode()) {
        if (curNode.getItem() > newItem) {
            addAfter(newNode, preNode);
            isInserted = true;
        }
        preNode = curNode;
    }
    if (!isInserted) //다 돌았는데 안들어갔으면 맨 뒤
        addAfter(newNode, preNode);
}

//키값으로 찾아 반환
template<typename T>
template<typename K>
T *MyLinkedList<T>::getByKey(const K &key) const {
    for (Node<T> curNode = head->getNextNode(); curNode != nullptr; curNode = curNode.getNextNode()) {
        if (curNode->getItem() == key)
            return curNode->getItem();
    }
    return nullptr; //다 돌았는데 없으면
}

//단방향 iterator 제작 후 반환
template<typename T>
MyIterator<T> MyLinkedList<T>::getIterator() const {
    MyIterator<T> listIterator = MyIterator<T>(head);
    return listIterator;
}

//키값 제거
template<typename T>
template<typename K>
bool MyLinkedList<T>::remove(const K &key) {
    Node<T>* preNode = head;
    for (Node<T> curNode = head->getNextNode(); curNode != nullptr; curNode = curNode.getNextNode()) {
        if (curNode.getItem() == key) { //찾았으면 지우고 true리턴
            removeAfter(preNode);
            return true;
        }
        preNode = curNode;
    }
    return false; //없으면 false
}

//private-----------------------------------------
//인자 뒤에 추가
template<typename T>
void MyLinkedList<T>::addAfter(Node<T> *newNode, Node<T> *preNode) {
    newNode->setNextNode(preNode->getNextNode());
    preNode->setNextNode(preNode);
}

//인자 뒤 삭제
template<typename T>
void MyLinkedList<T>::removeAfter(Node<T>* preNode) {
    Node<T>* target = preNode->getNextNode();
    preNode->setNextNode(target->getNextNode());
    delete target;
}

//재귀 기반 전부 삭제
template<typename T>
void MyLinkedList<T>::deleteAll(Node<T> *node) {
    if (node->getNextNode() != nullptr)
        deleteAll(node->getNextNode());
    delete node;
}
