#pragma once
#include "MySinglyLinkedList.h"

template<typename T>
MySinglyLinkedList<T>::MySinglyLinkedList(T *inputNode) : head(inputNode) {
}

template<typename T>
MySinglyLinkedList<T>::~MySinglyLinkedList() {
    T *current = head;

    //맨 앞부터 하나씩 지움
    while (current != nullptr) {
        //지우기 전에 다음 노드를 저장
        T *next = current->getNextNode();

        delete current;

        current = next;
    }

    head = nullptr;
}

template<typename T>
T *MySinglyLinkedList<T>::getHead() const { return head; }

template<typename T>
template<typename U>
bool MySinglyLinkedList<T>::sortedInsert(T *input, Comparator<U> &comparator) {
    //헤더가 없는 경우에 삽입하는 경우
    if (head == nullptr) {
        head = input;
        return true;
    }

    //맨 앞에 넣는 경우
    if (comparator.columnCompare(head->getObject(), input->getObject()) > 0) {
        input->linkNextNode(head);
        head = input;
        return true;
    }

    T *prev = nullptr;
    T *current = head;

    while (current != nullptr) {
        //들어가는 값이 작은 경우 -> 삽입
        if (comparator.columnCompare(current->getObject(), input->getObject()) > 0) {
            input->linkNextNode(current);
            prev->linkNextNode(input);

            return true;
        } else if (comparator.columnCompare(current->getObject(), input->getObject()) < 0) {
            prev = current;
            current = current->getNextNode();
        } else {
            //같은 값이 이미 있음 -> 넣지 않음
            return false;
        }
    }

    //끝까지 왔으면 맨 뒤에 붙임
    prev->linkNextNode(input);
    return true;
}

template<typename T>
T *MySinglyLinkedList<T>::remove(const string &input) {
    //헤드가 비어있는 경우
    if (head == nullptr) {
        return nullptr;
    }

    //헤드를 지워야 하는 경우
    if (head->getObject().getId() == input) {
        T *removed = head;

        head = head->getNextNode();
        removed->linkNextNode(nullptr);

        return removed;
    }

    T *prev = head;
    T *current = head->getNextNode();

    while (current != nullptr) {
        //값이 같은 경우
        if (current->getObject().getId() == input) {
            prev->linkNextNode(current->getNextNode());
            current->linkNextNode(nullptr);

            return current;
        }

        prev = current;
        current = current->getNextNode();
    }

    return nullptr;
}
