#ifndef GIFTED_H
#define GIFTED_H
#include "visitor/Visitable.h"

class Gifted final : public Visitable<Gifted> {
    MyString m_uid;
    MyString m_middle_school;
    MyString m_parent_name;
    MyString m_parent_phone_number;

    public:
        Gifted(const MyString &uid,
               const MyString &name, const MyString &course, const MyString &phone_number,
               const MyString &middle_school, const MyString &parent_name, const MyString &parent_phone_number);


        const MyString& uid() const override;

        const MyString& middle_school() const;

        const MyString& parent_name() const;

        const MyString& parent_phone_number() const;
};

#endif //GIFTED_H
