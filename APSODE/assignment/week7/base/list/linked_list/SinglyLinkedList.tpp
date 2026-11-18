#pragma once
#include "SinglyLinkedList.h"


template<typename E>
SinglyLinkedList<E>::SinglyLinkedList()
    : m_head(nullptr), m_tail(nullptr), m_size(0) {
}

template<typename E>
SinglyLinkedList<E>::SinglyLinkedList(const E *elements, int length)
    : m_head(nullptr), m_tail(nullptr), m_size(0) {
    this->SinglyLinkedList::add_all(elements, length);
}

template<typename E>
SinglyLinkedList<E>::SinglyLinkedList(const SinglyLinkedList &other)
    : m_head(nullptr), m_tail(nullptr), m_size(0) {
    for (SingleConnectNode<E> *current_node = other.m_head;
         current_node != nullptr;
         current_node = current_node->next()) {
        this->SinglyLinkedList::add_last(current_node->get());
    }
}

template<typename E>
SinglyLinkedList<E>::~SinglyLinkedList() {
    this->clear();
}

template<typename E>
void SinglyLinkedList<E>::add(const E &element) {
    this->add_last(element);
}

template<typename E>
void SinglyLinkedList<E>::add_all(const E *elements, int length) {
    require_positive(length);
    if (length == 0) {
        return;
    }
    require_non_null(elements);

    for (int element_index = 0; element_index < length; ++element_index) {
        this->add_last(elements[element_index]);
    }
}

template<typename E>
void SinglyLinkedList<E>::add_first(const E &element) {
    SingleConnectNode<E> *new_node = new SingleConnectNode<E>(element, this->m_head);

    this->m_head = new_node;
    if (this->m_tail == nullptr) {
        this->m_tail = new_node;
    }

    ++this->m_size;
}

template<typename E>
void SinglyLinkedList<E>::add_last(const E &element) {
    SingleConnectNode<E> *new_node = new SingleConnectNode<E>(element);

    if (this->m_tail == nullptr) {
        this->m_head = new_node;
    } else {
        this->m_tail->set_next(new_node);
    }
    this->m_tail = new_node;

    ++this->m_size;
}

template<typename E>
void SinglyLinkedList<E>::insert(int index, const E &element) {
    if (index < 0 || index > this->m_size) {
        throw std::out_of_range("index out of range");
    }

    if (index == 0) {
        this->add_first(element);
        return;
    }

    if (index == this->m_size) {
        this->add_last(element);
        return;
    }

    SingleConnectNode<E> *prev_node = this->node_at(index - 1);
    SingleConnectNode<E> *new_node = new SingleConnectNode<E>(element, prev_node->next());
    prev_node->set_next(new_node);

    ++this->m_size;
}

template<typename E>
const E& SinglyLinkedList<E>::peek(int index) const {
    this->require_valid_index(index);

    return this->node_at(index)->get();
}

template<typename E>
E SinglyLinkedList<E>::get(int index) const {
    this->require_valid_index(index);

    return this->node_at(index)->get();
}

template<typename E>
void SinglyLinkedList<E>::set(int index, const E &element) {
    this->require_valid_index(index);

    this->node_at(index)->set(element);
}

template<typename E>
E SinglyLinkedList<E>::remove(int index) {
    this->require_valid_index(index);

    SingleConnectNode<E> *removed_node;

    if (index == 0) {
        removed_node = this->m_head;
        this->m_head = removed_node->next();

        if (this->m_head == nullptr) {
            this->m_tail = nullptr;
        }
    } else {
        SingleConnectNode<E> *prev_node = this->node_at(index - 1);
        removed_node = prev_node->next();
        prev_node->set_next(removed_node->next());

        if (removed_node == this->m_tail) {
            this->m_tail = prev_node;
        }
    }

    E element = removed_node->get();
    delete removed_node;
    --this->m_size;

    return element;
}

template<typename E>
void SinglyLinkedList<E>::clear() {
    SingleConnectNode<E> *current_node = this->m_head;

    while (current_node != nullptr) {
        SingleConnectNode<E> *next_node = current_node->next();
        delete current_node;
        current_node = next_node;
    }

    this->m_head = nullptr;
    this->m_tail = nullptr;
    this->m_size = 0;
}

template<typename E>
int SinglyLinkedList<E>::size() const {
    return this->m_size;
}

template<typename E>
bool SinglyLinkedList<E>::empty() const {
    return this->m_size == 0;
}

template<typename E>
bool SinglyLinkedList<E>::contains(const E &object) const {
    for (SingleConnectNode<E> *current_node = this->m_head;
         current_node != nullptr;
         current_node = current_node->next()) {
        if (current_node->get() == object) {
            return true;
        }
    }

    return false;
}

template<typename E>
SinglyLinkedList<E>& SinglyLinkedList<E>::operator=(const SinglyLinkedList &other) {
    if (this == &other) {
        return *this;
    }

    SinglyLinkedList copied_list = SinglyLinkedList(other);
    SinglyLinkedList::swap(*this, copied_list);
    return *this;
}

template<typename E>
E& SinglyLinkedList<E>::operator[](int index) const {
    this->require_valid_index(index);
    return this->node_at(index)->get();
}

template<typename E>
E& SinglyLinkedList<E>::operator[](int index) {
    this->require_valid_index(index);
    return this->node_at(index)->get();
}

template<typename E>
void SinglyLinkedList<E>::require_valid_index(int index) const {
    if (!(0 <= index && index < this->m_size)) {
        throw std::out_of_range("index out of range");
    }
}

template<typename E>
SingleConnectNode<E>* SinglyLinkedList<E>::node_at(int index) const {
    SingleConnectNode<E> *current_node = this->m_head;

    for (int moved_count = 0; moved_count < index; ++moved_count) {
        current_node = current_node->next();
    }

    return current_node;
}

template<typename E>
void SinglyLinkedList<E>::swap(SinglyLinkedList &left_list, SinglyLinkedList &right_list) {
    SingleConnectNode<E> *temp_head = left_list.m_head;
    left_list.m_head = right_list.m_head;
    right_list.m_head = temp_head;

    SingleConnectNode<E> *temp_tail = left_list.m_tail;
    left_list.m_tail = right_list.m_tail;
    right_list.m_tail = temp_tail;

    int temp_size = left_list.m_size;
    left_list.m_size = right_list.m_size;
    right_list.m_size = temp_size;
}
