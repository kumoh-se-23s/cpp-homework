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

    this->container.remove(index);
    this->add(other);
}

void MySet::add(const int &other) {
    int index = this->binary_search(other);

    if (index < this->size() && this->container.get(index) == other) {
        return;
    }

    this->container.insert(index, other);
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
    int index = this->binary_search(object);

    return index < this->size() && this->container.get(index) == object;
}

void MySet::require_unique(const int &element) const {
    if (this->contains(element)) {
        throw std::invalid_argument("duplicate element");
    }
}

int MySet::binary_search(const int &target) const {
    return this->binary_search(target, 0, this->size());
}

int MySet::binary_search(const int &target, int low, int high) const {
    if (low >= high) {
        return low;
    }

    int mid = low + (high - low) / 2;

    if (this->container.get(mid) < target) {
        return this->binary_search(target, mid + 1, high);
    }
    return this->binary_search(target, low, mid);
}

MySet MySet::unionSet(const MySet &mySet) const {
    MySet result;
    int this_index = 0;
    int other_index = 0;

    for (; this_index < this->size() && other_index < mySet.size();) {
        int this_value = this->get(this_index);
        int other_value = mySet.get(other_index);

        result.container.add(this_value < other_value ? this_value : other_value);

        if (this_value <= other_value) {
            ++this_index;
        }
        if (other_value <= this_value) {
            ++other_index;
        }
    }

    for (; this_index < this->size(); ++this_index) {
        result.container.add(this->get(this_index));
    }
    for (; other_index < mySet.size(); ++other_index) {
        result.container.add(mySet.get(other_index));
    }

    return result;
}

MySet MySet::intersectionSet(const MySet &mySet) const {
    MySet result;

    for (int this_index = 0, other_index = 0; this_index < this->size() && other_index < mySet.size();) {
        int this_value = this->get(this_index);
        int other_value = mySet.get(other_index);

        if (this_value == other_value) {
            result.container.add(this_value);
        }
        if (this_value <= other_value) {
            ++this_index;
        }
        if (other_value <= this_value) {
            ++other_index;
        }
    }

    return result;
}

MySet MySet::differenceSet(const MySet &mySet) const {
    MySet result;
    int this_index = 0;
    int other_index = 0;

    for (; this_index < this->size() && other_index < mySet.size();) {
        int this_value = this->get(this_index);
        int other_value = mySet.get(other_index);

        if (this_value < other_value) {
            result.container.add(this_value);
        }
        if (this_value <= other_value) {
            ++this_index;
        }
        if (other_value <= this_value) {
            ++other_index;
        }
    }

    for (; this_index < this->size(); ++this_index) {
        result.container.add(this->get(this_index));
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

    while (input_stream >> element && element >= 0) {
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
