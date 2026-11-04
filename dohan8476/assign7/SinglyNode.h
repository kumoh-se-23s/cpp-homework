#pragma once

template <typename T>
struct SinglyNode{
    T data;
    SinglyNode* next;
};

template <typename T>
struct SinglyHeadNode{
    SinglyNode<T>* head = nullptr;
};