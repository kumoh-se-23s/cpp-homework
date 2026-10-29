#include "MySet.h"

#include <istream>
#include <ostream>
#include <stdexcept>

MySet::MySet()
    : container() {
}

MySet::MySet(const MySet &other)
    : container(other.container) {
}

MySet::~MySet() {
}

int MySet::get(int index) const {
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

int MySet::size() const {
    return this->container.size();
}

bool MySet::empty() const {
    return this->container.empty();
}

bool MySet::contains(const int &object) const {
    return this->container.contains(object);
}

void MySet::require_unique(const int &element) const {
    if (this->contains(element)) {
        throw std::invalid_argument("duplicate element");
    }
}

MySet MySet::unionSet(const MySet &mySet) const {
    MySet result(*this);

    for (int index = 0; index < mySet.size(); ++index) {
        result.add(mySet.get(index));
    }

    return result;
}

MySet MySet::intersectionSet(const MySet &mySet) const {
    MySet result;

    for (int index = 0; index < this->size(); ++index) {
        int element = this->get(index);

        if (mySet.contains(element)) {
            result.add(element);
        }
    }

    return result;
}

MySet MySet::differenceSet(const MySet &mySet) const {
    MySet result;

    for (int index = 0; index < this->size(); ++index) {
        int element = this->get(index);

        if (!mySet.contains(element)) {
            result.add(element);
        }
    }

    return result;
}

MySet MySet::operator+(const MySet &mySet) const {
    return this->unionSet(mySet);
}

MySet MySet::operator&(const MySet &mySet) const {
    return this->intersectionSet(mySet);
}

MySet MySet::operator-(const MySet &mySet) const {
    return this->differenceSet(mySet);
}

MySet &MySet::operator=(const MySet &mySet) {
    if (this == &mySet) {
        return *this;
    }

    this->container = mySet.container;
    return *this;
}

std::istream &operator>>(std::istream &input_stream, MySet &set) {
    int element;

    while (input_stream >> element && element != EOF) {
        set.add(element);
    }

    return input_stream;
}

std::ostream &operator<<(std::ostream &output_stream, const MySet &set) {
    output_stream << "{";

    for (int index = 0; index < set.size(); ++index) {
        if (index > 0) {
            output_stream << ", ";
        }

        output_stream << set.get(index);
    }

    return output_stream << "}";
}
