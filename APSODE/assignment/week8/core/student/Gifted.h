#ifndef GIFTED_H
#define GIFTED_H
#include "visitor/Visitable.h"

class Gifted final : public Visitable<Gifted> {
    MyString m_uid;
    MyString m_middle_school;
    MyString m_parent_name;
    MyString m_parent_phone_number;

    public:
        Gifted(const MyString &uid, const MyString &name, std::istream &input);


        const MyString& uid() const override;

        const MyString& middle_school() const;

        const MyString& parent_name() const;

        const MyString& parent_phone_number() const;


        void read_details(std::istream &input) override;

        void write(std::ostream &output) const override;
};

#endif //GIFTED_H
