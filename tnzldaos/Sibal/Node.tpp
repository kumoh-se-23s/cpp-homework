#pragma once

template<typename T>
Node<T>::Node(T* object) : object(object) {}

template<typename T>
Node<T>::~Node() { delete object; }

template<typename T>
const T &Node<T>::getObject() const { return *object; }

template<typename T>
Node<T> *Node<T>::getNextNode() const { return nextId; }

template<typename T>
void Node<T>::linkNextNode(Node *node) { nextId = node; }

template<typename T>
Node<T> *Node<T>::getNextTeamMember() const { return nextTeamMember; }

template<typename T>
Node<T> *Node<T>::getPrevTeamMember() const { return prevTeamMember; }

template<typename T>
void Node<T>::linkNextTeamMember(Node *node) { nextTeamMember = node; }

template<typename T>
void Node<T>::linkPrevTeamMember(Node *node) { prevTeamMember = node; }
