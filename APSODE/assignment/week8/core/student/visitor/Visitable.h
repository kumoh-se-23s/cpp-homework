#ifndef VISITABLE_H
#define VISITABLE_H
#include "StudentVisitor.h"
#include "../AdStudent.h"

template<typename Derived>
class Visitable : public AdStudent {
    public:
        void accept(StudentVisitor &visitor) const override;

    protected:
        explicit Visitable(const MyString &name);
};

#include "Visitable.tpp"

#endif //VISITABLE_H
