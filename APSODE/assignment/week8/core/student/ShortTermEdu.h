#ifndef SHORTTERMEDU_H
#define SHORTTERMEDU_H
#include "visitor/Visitable.h"
#include "mixin/AutoUID.h"

class ShortTermEdu final : public Visitable<ShortTermEdu>, AutoUID<ShortTermEdu, 'S'> {
    MyString m_uid;
    MyString m_host_depart;

    public:
        ShortTermEdu(const MyString &uid,
                     const MyString &name, const MyString &course, const MyString &phone_number,
                     const MyString &host_depart);


        static MyString issue_uid();


        const MyString& uid() const override;

        const MyString& host_depart() const;


        void release_uid() const override;
};

#endif //SHORTTERMEDU_H
