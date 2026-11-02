#include <stdexcept>

template<typename T, int TAIL>
Sequence<T, TAIL>::Sequence()
    : container(new T[DEFAULT_CAPACITY + 1]), m_capacity(DEFAULT_CAPACITY), m_size(0) {
}

template<typename T, int TAIL>
Sequence<T, TAIL>::Sequence(const T *elements, int size) : m_capacity(calc_new_capacity(size)), m_size(size) {
    this->copy_elements(elements, size);
}

template<typename T, int TAIL>
Sequence<T, TAIL>::Sequence(const Sequence &other) {
    this->m_size = other.m_size;
    this->m_capacity = other.m_capacity;
    this->copy_elements(other);
}

template<typename T, int TAIL>
Sequence<T, TAIL>::~Sequence() {
    delete[] this->container;
}


template<typename T, int TAIL>
int Sequence<T, TAIL>::size() const {
    return this->m_size;
}

template<typename T, int TAIL>
bool Sequence<T, TAIL>::empty() const {
    return this->m_size == 0;
}

template<typename T, int TAIL>
int Sequence<T, TAIL>::capacity() const {
    return this->m_capacity;
}


template<typename T, int TAIL>
const T& Sequence<T, TAIL>::at(int index) const {
    this->require_valid_index(index);
    return (*this)[index];
}

template<typename T, int TAIL>
T& Sequence<T, TAIL>::at(int index) {
    return const_cast<T&>(static_cast<const Sequence&>(*this).at(index));
}

template<typename T, int TAIL>
const T& Sequence<T, TAIL>::operator[](int index) const {
    return this->container[index];
}

template<typename T, int TAIL>
T& Sequence<T, TAIL>::operator[](int index) {
    return const_cast<T&>(static_cast<const Sequence&>(*this)[index]);
}


template<typename T, int TAIL>
void Sequence<T, TAIL>::ensure_capacity(int min_size) {
    if (this->m_capacity >= min_size) {
        return;
    }

    int new_capacity = calc_new_capacity(min_size);
    T *new_container = create_empty_container(new_capacity);
    for (int index = 0; index < min_size; ++index) {
        new_container[index] = this->container[index];
    }

    delete[] this->container;
    this->container = new_container;
    this->m_capacity = new_capacity;
    this->fill_tail();
}

template<typename T, int TAIL>
void Sequence<T, TAIL>::copy_elements(const Sequence &other) {
    Sequence::copy_elements(other.container, other.m_size);
}

template<typename T, int TAIL>
void Sequence<T, TAIL>::copy_elements(const T *elements, int size) {
    for (int index = 0; index < size; ++index) {
        this->container[index] = elements[index];
    }

    this->fill_tail();
}

template<typename T, int TAIL>
void Sequence<T, TAIL>::insert(int index, const T *element) {
}

template<typename T, int TAIL>
void Sequence<T, TAIL>::insert(int index, const T *elements, int count) {
}

template<typename T, int TAIL>
void Sequence<T, TAIL>::erase(int start, int count) {
}

template<typename T, int TAIL>
void Sequence<T, TAIL>::append(const T *element) {
}

template<typename T, int TAIL>
void Sequence<T, TAIL>::append(const T *elements, int count) {
}

template<typename T, int TAIL>
void Sequence<T, TAIL>::clear() {
    this->m_capacity = DEFAULT_CAPACITY;
    this->m_size = 0;
    delete[] this->container;
    this->container = Sequence::create_empty_container(this->m_size);
}

template<typename T, int TAIL>
Sequence<T, TAIL>& Sequence<T, TAIL>::operator=(const Sequence &other) {
    if (this == &other) {
        return *this;
    }

    Sequence temp = Sequence(other);
    Sequence::swap(*this, temp);
    return *this;
}

template<typename T, int TAIL>
int Sequence<T, TAIL>::extend(int expected) {
}

template<typename T, int TAIL>
void Sequence<T, TAIL>::fill_tail() {
    for (int index = 0; index < TAIL; ++index) {
        this->container[this->m_size + index] = T();
    }
}

template<typename T, int TAIL>
void Sequence<T, TAIL>::require_valid_index(int index) const {
    if (!(0 <= index && index < this->m_size)) {
        throw std::out_of_range("Sequence: index is out of range");
    }
}

template<typename T, int TAIL>
void Sequence<T, TAIL>::require_valid_range(const T *elements, int count) {
    if (count < 0) {
        throw std::invalid_argument("Sequence: count must not be negative");
    }

    if (elements == nullptr) {
        throw std::invalid_argument("Sequence: elements must not be null");
    }
}



