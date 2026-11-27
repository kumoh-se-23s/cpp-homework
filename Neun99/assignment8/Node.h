#pragma once

template <typename T>
class Node {
public:
    Node(const T* item);
    ~Node();

    //getter
    T* getItem() const;
    Node* getNextNode() const;

    //setter
    void setItem(const T* item);
    void setNextNode(const Node* nextNode);

private:
    T* item;
    Node* nextNode;
};