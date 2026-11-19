#pragma once
#include "Linker.h"
#include "../util/RequireGuard.h"


template<typename Node, LinkPolicyFor<Node> LinkPolicy, NodeComparator<Node> ComparePolicy>
Linker<Node, LinkPolicy, ComparePolicy>::Linker()
    : m_head(nullptr), m_size(0) {
}

template<typename Node, LinkPolicyFor<Node> LinkPolicy, NodeComparator<Node> ComparePolicy>
Linker<Node, LinkPolicy, ComparePolicy>::~Linker() {
}

template<typename Node, LinkPolicyFor<Node> LinkPolicy, NodeComparator<Node> ComparePolicy>
Node* Linker<Node, LinkPolicy, ComparePolicy>::head() const {
    return this->m_head;
}

template<typename Node, LinkPolicyFor<Node> LinkPolicy, NodeComparator<Node> ComparePolicy>
int Linker<Node, LinkPolicy, ComparePolicy>::size() const {
    return this->m_size;
}

template<typename Node, LinkPolicyFor<Node> LinkPolicy, NodeComparator<Node> ComparePolicy>
bool Linker<Node, LinkPolicy, ComparePolicy>::empty() const {
    return this->m_size == 0;
}

template<typename Node, LinkPolicyFor<Node> LinkPolicy, NodeComparator<Node> ComparePolicy>
bool Linker<Node, LinkPolicy, ComparePolicy>::contains(const Node *node) const {
    return this->find_if([node](const Node *current_node) {
        return current_node == node;
    }) != nullptr;
}

template<typename Node, LinkPolicyFor<Node> LinkPolicy, NodeComparator<Node> ComparePolicy>
void Linker<Node, LinkPolicy, ComparePolicy>::insert(Node *node) {
    require_non_null(node);

    Node *insert_position = this->find_insert_position(node);

    if (insert_position == nullptr) {
        LinkPolicy::link_front(this->m_head, node);
    } else {
        LinkPolicy::link_after(insert_position, node);
    }

    ++this->m_size;
}

template<typename Node, LinkPolicyFor<Node> LinkPolicy, NodeComparator<Node> ComparePolicy>
void Linker<Node, LinkPolicy, ComparePolicy>::remove(Node *node) {
    require_non_null(node);

    LinkPolicy::unlink(this->m_head, node);

    --this->m_size;
}

template<typename Node, LinkPolicyFor<Node> LinkPolicy, NodeComparator<Node> ComparePolicy>
void Linker<Node, LinkPolicy, ComparePolicy>::clear() {
    this->for_each([](Node *node) {
        LinkPolicy::reset(node);
    });

    this->m_head = nullptr;
    this->m_size = 0;
}

template<typename Node, LinkPolicyFor<Node> LinkPolicy, NodeComparator<Node> ComparePolicy>
void Linker<Node, LinkPolicy, ComparePolicy>::take(Linker &other) {
    if (this == &other) {
        return;
    }

    this->clear();

    this->m_head = other.m_head;
    this->m_size = other.m_size;

    other.m_head = nullptr;
    other.m_size = 0;
}

template<typename Node, LinkPolicyFor<Node> LinkPolicy, NodeComparator<Node> ComparePolicy>
Node* Linker<Node, LinkPolicy, ComparePolicy>::find_if(std::function<bool(const Node *node)> pred) const {
    for (Node *current_node = this->m_head; current_node != nullptr; current_node = Linker::step(current_node, this->m_head)) {
        if (pred(current_node)) {
            return current_node;
        }
    }

    return nullptr;
}

template<typename Node, LinkPolicyFor<Node> LinkPolicy, NodeComparator<Node> ComparePolicy>
void Linker<Node, LinkPolicy, ComparePolicy>::for_each(std::function<void(Node *node)> visit) const {
    const Node *start_node = this->m_head;
    for (Node *current_node = this->m_head, *next_node = nullptr; current_node != nullptr; current_node = next_node) {
        next_node = Linker::step(current_node, start_node);
        visit(current_node);
    }
}

template<typename Node, LinkPolicyFor<Node> LinkPolicy, NodeComparator<Node> ComparePolicy>
void Linker<Node, LinkPolicy, ComparePolicy>::for_each(const Node *start, std::function<void(const Node *node)> visit) const {
    require_non_null(start);

    for (const Node *current_node = Linker::step(start, start); current_node != nullptr; current_node = Linker::step(current_node, start)) {
        visit(current_node);
    }
}

template<typename Node, LinkPolicyFor<Node> LinkPolicy, NodeComparator<Node> ComparePolicy>
Node* Linker<Node, LinkPolicy, ComparePolicy>::find_insert_position(const Node *node) const {
    if (this->m_head == nullptr || ComparePolicy::less(node, this->m_head)) {
        return nullptr;
    }

    Node *insert_position = this->m_head;
    for (Node *current_node = Linker::step(this->m_head, this->m_head); current_node != nullptr && !ComparePolicy::less(node, current_node); current_node = Linker::step(current_node, this->m_head)) {
        insert_position = current_node;
    }

    return insert_position;
}

template<typename Node, LinkPolicyFor<Node> LinkPolicy, NodeComparator<Node> ComparePolicy>
Node* Linker<Node, LinkPolicy, ComparePolicy>::step(const Node *node, const Node *stop) {
    Node *next_node = LinkPolicy::next(node);

    if (next_node == stop) {
        return nullptr;
    }

    return next_node;
}
