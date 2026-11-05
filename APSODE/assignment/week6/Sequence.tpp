#include <stdexcept>


template<typename T, int TAIL>
Sequence<T, TAIL>::Sequence()
    : container(new T[DEFAULT_CAPACITY + 1]()), m_capacity(DEFAULT_CAPACITY), m_size(0) {
}

template<typename T, int TAIL>
Sequence<T, TAIL>::Sequence(const T *elements, int size) : m_capacity(calc_new_capacity(size)), m_size(size) {
    Sequence::require_non_null(elements);
    Sequence::require_valid_amount(size);
    this->container = Sequence::create_empty_container(Sequence::calc_new_capacity(this->m_size));
    Sequence::copy_array(
        elements, size,
        this->container, this->m_capacity
    );
}

template<typename T, int TAIL>
Sequence<T, TAIL>::Sequence(const Sequence &other) : m_capacity(other.m_capacity), m_size(other.m_size) {
    this->container = Sequence::create_empty_container(other.m_capacity);
    Sequence::copy_array(
        other.container, other.m_capacity,
        this->container, this->m_capacity
    );
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
void Sequence<T, TAIL>::insert(int position, const T &element) {
    this->insert(position, &element, 1);
}

template<typename T, int TAIL>
void Sequence<T, TAIL>::insert(int position, const T *elements, int amount) {
    this->require_valid_position(position);
    Sequence::require_non_null(elements);
    Sequence::require_valid_amount(amount);

    if (elements == this->container) {
        T* temp = new T[this->m_capacity + 1];
        copy_array(elements, amount, temp, this->m_capacity);
        elements = temp;
    }

    if (this->need_extend(amount)) {
        this->extend(this->m_size + amount);
    }

    for (int cursor = this->m_size - 1; cursor >= position; --cursor) {
        this->container[cursor + amount] = this->container[cursor];
    }

    for (T* pointer = this->container + position; pointer < this->container + position + amount; ++pointer) {
        *pointer = *(elements++);
    }

    this->m_size += amount;
    this->fill_tail();
}

template<typename T, int TAIL>
void Sequence<T, TAIL>::append(const T &element) {
    this->append(&element, 1);
}

template<typename T, int TAIL>
void Sequence<T, TAIL>::append(const T *elements, int amount) {
    Sequence::require_non_null(elements);
    Sequence::require_valid_amount(amount);

    if (elements == this->container) {
        T* temp = new T[this->m_capacity + 1];
        copy_array(elements, amount, temp, this->m_capacity);
        elements = temp;
    }

    if (this->need_extend(amount)) {
        this->extend(this->m_size + amount);
    }

    for (T* cursor = this->container + this->m_size; cursor < this->container + this->m_size + amount; ++cursor) {
        *cursor = *(elements++);
    }

    this->m_size += amount;
    this->fill_tail();
}

template<typename T, int TAIL>
void Sequence<T, TAIL>::erase(int start, int amount) {
    if (start < 0) {
        start = 0;
    }

    if (start > this->m_size) {
        start = this->m_size;
    }

    if (amount < 0 || amount > this->m_size - start) {
        amount = this->m_size - start;
    }

    for (int index = start; index < this->m_size - amount; ++index) {
        this->container[index] = this->container[index + amount];
    }

    for (int index = this->m_size - amount; index < this->m_size; ++index) {
        this->container[index] = T();
    }

    this->m_size -= amount;
    this->fill_tail();

    if (this->need_shrink()) {
        this->shrink();
    }
}

template<typename T, int TAIL>
bool Sequence<T, TAIL>::equals(const Sequence &other) const {
    if (this->m_size != other.m_size) {
        return false;
    }

    for (int index = 0; index < this->m_size; ++index) {
        if (this->container[index] != other.container[index]) {
            return false;
        }
    }

    return true;
}

// 0 1 2 3 4 5
// 2, 3

template<typename T, int TAIL>
void Sequence<T, TAIL>::clear() {
    this->m_capacity = DEFAULT_CAPACITY;
    this->m_size = 0;
    delete[] this->container; // 굳이 delete를 할까 싶긴한데... 일단은 초기 상태의 컨테이너를 가지는게 맞다고 생각하기에...
    this->container = Sequence::create_empty_container(DEFAULT_CAPACITY);
}

template<typename T, int TAIL>
Sequence<T, TAIL>& Sequence<T, TAIL>::operator=(const Sequence &other) {
    if (this == &other) {
        return *this;
    }

    Sequence temp = Sequence(other);
    swap(*this, temp);
    return *this;
}

template<typename T, int TAIL>
void Sequence<T, TAIL>::shrink() {
    int new_capacity = Sequence::calc_new_capacity(this->m_capacity / GROWTH_FACTOR);
    T* new_container = Sequence::create_empty_container(new_capacity);

    copy_array(
        this->container, this->m_size,
        new_container, new_capacity
    );
    delete[] this->container;
    this->container = new_container;
    this->m_capacity = new_capacity;
}

template<typename T, int TAIL>
void Sequence<T, TAIL>::extend(int required) {
    int grown = this->m_capacity * GROWTH_FACTOR;
    int new_capacity = Sequence::calc_new_capacity(grown > required ? grown : required);
    T* new_container = Sequence::create_empty_container(new_capacity);

    copy_array(
        this->container, this->m_capacity,
        new_container, new_capacity
    );
    delete[] this->container;
    this->container = new_container;
    this->m_capacity = new_capacity;
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

// insert 단일 사용이지만 일단 require_valid_index와의 통일성을 위해...
template<typename T, int TAIL>
void Sequence<T, TAIL>::require_valid_position(int position) const {
    if (!(0 <= position && position <= this->m_size)) {
        throw std::out_of_range("Sequence: position is out of range");
    }
}

template<typename E, int TAIL>
void Sequence<E, TAIL>::require_non_null(const E *nullable) {
    if (nullable == nullptr) {
        throw std::invalid_argument("Sequence: elements must not be null");
    }
}

template<typename T, int TAIL>
void Sequence<T, TAIL>::require_valid_amount(int amount) {
    if (amount < 0) {
        throw std::invalid_argument("Sequence: amount must not be negative");
    }
}



