#include "DoublyLinkedList.h"


template<typename E>
DoublyLinkedList<E>::DoublyLinkedList()
    : m_head(nullptr), m_size(0) {
}

template<typename E>
DoublyLinkedList<E>::DoublyLinkedList(const E *elements, int length)
    : m_head(nullptr), m_size(0) {
    this->DoublyLinkedList::add_all(elements, length);
}

template<typename E>
DoublyLinkedList<E>::DoublyLinkedList(const DoublyLinkedList &other)
    : m_head(nullptr), m_size(0) {
    DoubleConnectNode<E> *current_node = other.m_head;

    for (int copied_count = 0; copied_count < other.m_size; ++copied_count) {
        this->DoublyLinkedList::add_last(current_node->get());
        current_node = current_node->next();
    }
}

template<typename E>
DoublyLinkedList<E>::~DoublyLinkedList() {
    this->clear();
}

template<typename E>
void DoublyLinkedList<E>::add(const E &element) {
    this->add_last(element);
}

template<typename E>
void DoublyLinkedList<E>::add_all(const E *elements, int length) {
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
void DoublyLinkedList<E>::add_first(const E &element) {
    DoubleConnectNode<E> *new_node = new DoubleConnectNode<E>(element);

    if (this->m_head == nullptr) {
        this->link_first_node(new_node);
    } else {
        this->link_before(this->m_head, new_node);
        this->m_head = new_node;
    }

    ++this->m_size;
}

template<typename E>
void DoublyLinkedList<E>::add_last(const E &element) {
    DoubleConnectNode<E> *new_node = new DoubleConnectNode<E>(element);

    if (this->m_head == nullptr) {
        this->link_first_node(new_node);
    } else {
        this->link_before(this->m_head, new_node);
    }

    ++this->m_size;
}

template<typename E>
void DoublyLinkedList<E>::insert(int index, const E &element) {
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

    DoubleConnectNode<E> *target_node = this->node_at(index);
    DoubleConnectNode<E> *new_node = new DoubleConnectNode<E>(element);
    this->link_before(target_node, new_node);

    ++this->m_size;
}

template<typename E>
const E& DoublyLinkedList<E>::peek(int index) const {
    this->require_valid_index(index);

    return this->node_at(index)->get();
}

template<typename E>
E DoublyLinkedList<E>::get(int index) const {
    this->require_valid_index(index);

    return this->node_at(index)->get();
}

template<typename E>
void DoublyLinkedList<E>::set(int index, const E &element) {
    this->require_valid_index(index);

    this->node_at(index)->set(element);
}

template<typename E>
E DoublyLinkedList<E>::remove(int index) {
    this->require_valid_index(index);

    DoubleConnectNode<E> *removed_node = this->node_at(index);

    if (this->m_size == 1) {
        this->m_head = nullptr;
    } else {
        removed_node->prev()->set_next(removed_node->next());
        removed_node->next()->set_prev(removed_node->prev());

        if (removed_node == this->m_head) {
            this->m_head = removed_node->next();
        }
    }

    E element = removed_node->get();
    delete removed_node;
    --this->m_size;

    return element;
}

template<typename E>
void DoublyLinkedList<E>::clear() {
    DoubleConnectNode<E> *current_node = this->m_head;

    for (int deleted_count = 0; deleted_count < this->m_size; ++deleted_count) {
        DoubleConnectNode<E> *next_node = current_node->next();
        delete current_node;
        current_node = next_node;
    }

    this->m_head = nullptr;
    this->m_size = 0;
}

template<typename E>
int DoublyLinkedList<E>::size() const {
    return this->m_size;
}

template<typename E>
bool DoublyLinkedList<E>::empty() const {
    return this->m_size == 0;
}

template<typename E>
bool DoublyLinkedList<E>::contains(const E &object) const {
    DoubleConnectNode<E> *current_node = this->m_head;

    for (int visited_count = 0; visited_count < this->m_size; ++visited_count) {
        if (current_node->get() == object) {
            return true;
        }
        current_node = current_node->next();
    }

    return false;
}

template<typename E>
DoublyLinkedList<E>& DoublyLinkedList<E>::operator=(const DoublyLinkedList &other) {
    if (this == &other) {
        return *this;
    }

    DoublyLinkedList copied_list = DoublyLinkedList(other);
    DoublyLinkedList::swap(*this, copied_list);
    return *this;
}

template<typename E>
E& DoublyLinkedList<E>::operator[](int index) const {
    this->require_valid_index(index);
    return this->node_at(index)->get();
}

template<typename E>
E& DoublyLinkedList<E>::operator[](int index) {
    this->require_valid_index(index);
    return this->node_at(index)->get();
}

template<typename E>
void DoublyLinkedList<E>::require_valid_index(int index) const {
    if (!(0 <= index && index < this->m_size)) {
        throw std::out_of_range("index out of range");
    }
}

template<typename E>
DoubleConnectNode<E>* DoublyLinkedList<E>::node_at(int index) const {
    DoubleConnectNode<E> *current_node = this->m_head;

    if (index < this->m_size / 2) {
        for (int moved_count = 0; moved_count < index; ++moved_count) {
            current_node = current_node->next();
        }
    } else {
        for (int moved_count = 0; moved_count < this->m_size - index; ++moved_count) {
            current_node = current_node->prev();
        }
    }

    return current_node;
}

template<typename E>
void DoublyLinkedList<E>::link_first_node(DoubleConnectNode<E> *new_node) {
    new_node->set_next(new_node);
    new_node->set_prev(new_node);
    this->m_head = new_node;
}

template<typename E>
void DoublyLinkedList<E>::link_before(DoubleConnectNode<E> *target_node, DoubleConnectNode<E> *new_node) {
    DoubleConnectNode<E> *prev_node = target_node->prev();

    new_node->set_prev(prev_node);
    new_node->set_next(target_node);
    prev_node->set_next(new_node);
    target_node->set_prev(new_node);
}

template<typename E>
void DoublyLinkedList<E>::swap(DoublyLinkedList &left_list, DoublyLinkedList &right_list) {
    DoubleConnectNode<E> *temp_head = left_list.m_head;
    left_list.m_head = right_list.m_head;
    right_list.m_head = temp_head;

    int temp_size = left_list.m_size;
    left_list.m_size = right_list.m_size;
    right_list.m_size = temp_size;
}
