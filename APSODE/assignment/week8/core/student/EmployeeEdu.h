#ifndef EMPLOYEEEDU_H
#define EMPLOYEEEDU_H
#include "visitor/Visitable.h"
#include "mixin/AutoUID.h"

class EmployeeEdu final : public Visitable<EmployeeEdu>, AutoUID<EmployeeEdu, 'E'> {
    MyString m_uid;
    MyString m_company;
    MyString m_position;

    public:
        EmployeeEdu(const MyString &uid,
                    const MyString &name, const MyString &course, const MyString &phone_number,
                    const MyString &company, const MyString &position);


        static MyString issue_uid();


        const MyString& uid() const override;

        const MyString& company() const;

        const MyString& position() const;


        void release_uid() const override;
};

#endif //EMPLOYEEEDU_H
