#pragma once
#include "Visitable.h"

template<typename Derived>
Visitable<Derived>::Visitable(const MyString &name, const MyString &course, const MyString &phone_number)
    : AdStudent(name, course, phone_number) {
}

template<typename Derived>
void Visitable<Derived>::accept(StudentVisitor &visitor) const {
    visitor.visit(static_cast<const Derived&>(*this));
}
