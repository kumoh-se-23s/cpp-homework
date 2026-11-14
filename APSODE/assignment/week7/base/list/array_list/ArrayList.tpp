#include "ArrayList.h"


template<typename E>
ArrayList<E>::ArrayList()
    : container(new E[DEFAULT_CAPACITY]), m_size(0), m_capacity(DEFAULT_CAPACITY) {
}

template<typename E>
ArrayList<E>::ArrayList(int manual_capacity)
    : m_size(0), m_capacity(manual_capacity) {
    if (manual_capacity < 1) {
        throw std::invalid_argument("capacity must be positive");
    }

    this->container = new E[manual_capacity];
}

template<typename E>
ArrayList<E>::ArrayList(const E *elements, int count)
    : container(new E[DEFAULT_CAPACITY]), m_size(0), m_capacity(DEFAULT_CAPACITY) {
    this->ArrayList::add_all(elements, count);
}

template<typename E>
ArrayList<E>::ArrayList(const ArrayList &other)
    : container(new E[other.m_capacity]), m_size(other.m_size), m_capacity(other.m_capacity) {
    for (int index = 0; index < this->m_size; ++index) {
        this->container[index] = other.container[index];
    }
}

template<typename E>
ArrayList<E>& ArrayList<E>::operator=(const ArrayList &other) {
    if (this == &other) {
        return *this;
    }

    E *new_container = new E[other.m_capacity];
    for (int index = 0; index < other.m_size; ++index) {
        new_container[index] = other.container[index];
    }

    delete[] this->container;
    this->container = new_container;
    this->m_size = other.m_size;
    this->m_capacity = other.m_capacity;
    return *this;
}

template<typename E>
ArrayList<E>::~ArrayList() {
    delete[] this->container;
}

template<typename E>
E ArrayList<E>::get(int index) const {
    this->require_valid_index(index);

    return this->container[index];
}

template<typename E>
void ArrayList<E>::set(int index, const E &other) {
    this->require_valid_index(index);

    this->container[index] = other;
}

template<typename E>
const E& ArrayList<E>::peek(int index) const {
    this->require_valid_index(index);

    return this->container[index];
}

template<typename E>
void ArrayList<E>::add(const E &other) {
    if (this->need_expansion()) {
        this->expand();
    }

    this->container[this->m_size++] = other;
}

template<typename E>
void ArrayList<E>::add_all(const E *others, int count) {
    if (count < 0) {
        throw std::invalid_argument("negative element count");
    }

    for (int index = 0; index < count; ++index) {
        this->add(others[index]);
    }
}

template<typename E>
void ArrayList<E>::add_first(const E &other) {
    this->insert(0, other);
}

template<typename E>
void ArrayList<E>::add_last(const E &other) {
    this->add(other);
}

template<typename E>
void ArrayList<E>::insert(int index, const E &other) {
    if (index < 0 || index > this->m_size) {
        throw std::out_of_range("index out of range");
    }

    if (this->need_expansion()) {
        this->expand();
    }

    for (int container_index = this->m_size; container_index > index; --container_index) {
        this->container[container_index] = this->container[container_index - 1];
    }

    this->container[index] = other;
    ++this->m_size;
}

template<typename E>
E ArrayList<E>::remove(int index) {
    this->require_valid_index(index);

    E element = this->container[index];

    for (int container_index = index; container_index < this->m_size - 1; ++container_index) {
        this->container[container_index] = this->container[container_index + 1];
    }
    --this->m_size;

    if (this->need_shrink()) {
        this->shrink();
    }

    return element;
}

template<typename E>
int ArrayList<E>::size() const {
    return this->m_size;
}

template<typename E>
bool ArrayList<E>::empty() const {
    return this->m_size == 0;
}

template<typename E>
bool ArrayList<E>::contains(const E &object) const {
    for (int index = 0; index < this->m_size; ++index) {
        if (this->container[index] == object) {
            return true;
        }
    }

    return false;
}

template<typename E>
void ArrayList<E>::swap(int index1, int index2) {
    this->require_valid_index(index1);
    this->require_valid_index(index2);

    E temp = this->container[index1];
    this->container[index1] = this->container[index2];
    this->container[index2] = temp;
}

template<typename E>
E& ArrayList<E>::operator[](int index) {
    this->require_valid_index(index);
    return this->container[index];
}

template<typename E>
E& ArrayList<E>::operator[](int index) const {
    this->require_valid_index(index);
    return this->container[index];
}

template<typename E>
void ArrayList<E>::require_valid_index(int index) const {
    if (!(0 <= index && index < this->m_size)) {
        throw std::out_of_range("index out of range");
    }
}

template<typename E>
bool ArrayList<E>::need_expansion(int add_amount) {
    return this->m_size + add_amount > this->m_capacity;
}

template<typename E>
bool ArrayList<E>::need_shrink() {
    return this->m_capacity > DEFAULT_CAPACITY
           && this->m_size <= this->m_capacity * DEFAULT_SHRINK_CONDITION;
}

template<typename E>
void ArrayList<E>::expand() {
    this->resize(this->m_capacity * DEFAULT_EXPAND_RATIO);
}

template<typename E>
void ArrayList<E>::shrink() {
    int new_capacity = static_cast<int>(this->m_capacity * DEFAULT_SHRINK_RATIO);
    if (new_capacity < DEFAULT_CAPACITY) {
        new_capacity = DEFAULT_CAPACITY;
    }

    this->resize(new_capacity);
}

template<typename E>
void ArrayList<E>::resize(int new_capacity) {
    if (new_capacity < this->m_size) {
        throw std::invalid_argument("capacity smaller than size");
    }

    E *new_container = new E[new_capacity];
    for (int index = 0; index < this->m_size; ++index) {
        new_container[index] = this->container[index];
    }

    delete[] this->container;
    this->container = new_container;
    this->m_capacity = new_capacity;
}
