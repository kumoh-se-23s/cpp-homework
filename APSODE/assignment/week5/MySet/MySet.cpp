//
// Created by apsode on 26. 10. 13..
//

#include "MySet.h"

template<typename E>
MySet<E>::MySet(const E *array) : container(array){
}

template<typename E>
MySet<E>::MySet(const MySet &set) : container(set.container){
}

template<typename E>
MySet<E>::~MySet() {
}

template<typename E>
E* MySet<E>::begin() {
    return this->container.begin();
}

template<typename E>
E* MySet<E>::end() {
    return this->container.end();
}

template<typename E>
const E* MySet<E>::begin() const {
    return this->container.begin();
}

template<typename E>
const E* MySet<E>::end() const {
    return this->container.end();
}

template<typename E>
int MySet<E>::get_size() const {
    return this->container.get_size();
}

template<typename E>
int MySet<E>::get_capacity() const {
    return this->container.get_capacity();
}

template<typename E>
int MySet<E>::add(const E &value) {
    this->container.add(value);
}

template<typename E>
int MySet<E>::insert(int index, const E &value) {
    this->container.insert(index, value);
}

template<typename E>
int MySet<E>::remove_at(int index) {
    this->container.remove_at(index);
}

template<typename E>
bool MySet<E>::is_empty() {
    this->container.is_empty();
}

template<typename E>
bool MySet<E>::is_contains(const E &value) {
    this->container.is_contains(value);
}
