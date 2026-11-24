#pragma once

template<typename T>
class OldNode;

template<typename T>
OldNode<T> &OldNode<T>::operator=(const OldNode &other) {
    if (this != &other) {
        this->item = other.getItem();
    }

    return *this;
}

template<typename T>
OldNode<T> & OldNode<T>::operator=(OldNode &&other) noexcept {
    if (this != &other) {
        this->item = std::move(other.item);
    }
    return *this;
}

template<typename T>
const T & OldNode<T>::getItem() const {
    return item;
}

template<typename T>
T & OldNode<T>::getItem() {
    return item;
}

template<typename T>
void OldNode<T>::setItem(const T &item) {
    this->item = item;
}
template <typename T>
    OOldMyString OldNode<T>::toString() const requires HasToString<T> {
    return item.toString();
}
template<typename T>
std::ostream &operator<<(std::ostream &out, const OldNode<T> &node) {
    out << node.getItem();
    return out;
}
