#include "MySet.h"

#include <istream>
#include <stdexcept>

MySet::~MySet() {
}

int MySet::get(int index) {
    return this->container.get(index);
}

void MySet::set(int index, const int &other) {
    if (this->container.get(index) == other) {
        return;
    }

    this->require_unique(other);

    this->container.set(index, other);
}

void MySet::add(const int &other) {
    if (!this->container.contains(other)) {
        this->container.add(other);
    }
}

void MySet::add_all(const int *others, int count) {
    if (count < 0) {
        throw std::invalid_argument("negative element count");
    }

    for (int index = 0; index < count; ++index) {
        this->add(others[index]);
    }
}

int MySet::remove(int index) {
    return this->container.remove(index);
}

int MySet::size() {
    return this->container.size();
}

bool MySet::empty() {
    return this->container.empty();
}

bool MySet::contains(const int &object) {
    return this->container.contains(object);
}

void MySet::require_unique(const int &element) {
    if (this->contains(element)) {
        throw std::invalid_argument("duplicate element");
    }
}

MySet MySet::unionSet(const MySet &mySet) const {
}

MySet MySet::intersectionSet(const MySet &mySet) const {
}

MySet MySet::differenceSet(const MySet &mySet) const {
}

MySet MySet::operator+(const MySet &mySet) const {
}

MySet MySet::operator&(const MySet &mySet) const {
}

MySet MySet::operator-(const MySet &mySet) const {
}

MySet & MySet::operator=(const MySet &mySet) {
}

std::istream & operator>>(std::istream &input_stream, MySet &set) {
    for ( )
}

std::ostream & operator<<(std::ostream &input_stream, const MySet &set) {

}
