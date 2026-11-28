#ifndef VISITABLE_H
#define VISITABLE_H
#include "StudentVisitor.h"
#include "../AdStudent.h"

template<typename Derived>
class Visitable : public AdStudent {
    public:
        void accept(StudentVisitor &visitor) const override;

    protected:
        Visitable(const MyString &name, const MyString &course, const MyString &phone_number);
};

#include "Visitable.tpp"

#endif //VISITABLE_H
