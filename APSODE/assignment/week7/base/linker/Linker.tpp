#pragma once
#include <optional>

#include "Linker.h"

template<typename Node, typename LinkPolicy, typename ComparePolicy>
Linker<Node, LinkPolicy, ComparePolicy>::Linker()
    : m_head(nullptr), m_size(0) {
}

template<typename Node, typename LinkPolicy, typename ComparePolicy>
Linker<Node, LinkPolicy, ComparePolicy>::~Linker() {
}

template<typename Node, typename LinkPolicy, typename ComparePolicy>
Node* Linker<Node, LinkPolicy, ComparePolicy>::head() const {
    return this->m_head;
}

template<typename Node, typename LinkPolicy, typename ComparePolicy>
int Linker<Node, LinkPolicy, ComparePolicy>::size() const {
    return this->m_size;
}

template<typename Node, typename LinkPolicy, typename ComparePolicy>
bool Linker<Node, LinkPolicy, ComparePolicy>::empty() const {
    return this->m_size == 0;
}

template<typename Node, typename LinkPolicy, typename ComparePolicy>
bool Linker<Node, LinkPolicy, ComparePolicy>::contains(const Node *node) const {
    return this->find(node) != nullptr;
}

template<typename Node, typename LinkPolicy, typename ComparePolicy>
void Linker<Node, LinkPolicy, ComparePolicy>::insert(Node *node) {
}

template<typename Node, typename LinkPolicy, typename ComparePolicy>
void Linker<Node, LinkPolicy, ComparePolicy>::remove(Node *node) {
}

template<typename Node, typename LinkPolicy, typename ComparePolicy>
void Linker<Node, LinkPolicy, ComparePolicy>::clear() {
}

template<typename Node, typename LinkPolicy, typename ComparePolicy>
void Linker<Node, LinkPolicy, ComparePolicy>::take(Linker &other) {
}

template<typename Node, typename LinkPolicy, typename ComparePolicy>
Node* Linker<Node, LinkPolicy, ComparePolicy>::find(std::function<bool(Node *node)> predicate) const {
}

template<typename Node, typename LinkPolicy, typename ComparePolicy>
void Linker<Node, LinkPolicy, ComparePolicy>::for_each(std::function<void(Node *node)> consumer) const {
}

template<typename Node, typename LinkPolicy, typename ComparePolicy>
void Linker<Node, LinkPolicy, ComparePolicy>::for_each(const Node *start, std::function<bool(Node *node)> consumer) const {
}

template<typename Node, typename LinkPolicy, typename ComparePolicy>
Node* Linker<Node, LinkPolicy, ComparePolicy>::find_insert_pos(const Node *target) const {
}

template<typename Node, typename LinkPolicy, typename ComparePolicy>
Node* Linker<Node, LinkPolicy, ComparePolicy>::step(const Node *node, const Node *stop) {
}
