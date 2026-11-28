#ifndef LIFELONG_H
#define LIFELONG_H
#include "visitor/Visitable.h"

class LifeLong final : public Visitable<LifeLong> {
    MyString m_uid;
    int m_tuition;

    public:
        LifeLong(const MyString &uid,
                 const MyString &name, const MyString &course, const MyString &phone_number,
                 int tuition);


        const MyString& uid() const override;

        int tuition() const;
};

#endif //LIFELONG_H
