#pragma once
#include "Visitable.h"

template<typename Derived>
Visitable<Derived>::Visitable(const MyString &name)
    : AdStudent(name) {
}

template<typename Derived>
void Visitable<Derived>::accept(StudentVisitor &visitor) const {
    visitor.visit(static_cast<const Derived&>(*this));
}
