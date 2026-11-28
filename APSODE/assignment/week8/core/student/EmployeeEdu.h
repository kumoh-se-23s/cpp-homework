#ifndef EMPLOYEEEDU_H
#define EMPLOYEEEDU_H
#include "visitor/Visitable.h"
#include "mixin/AutoUID.h"

class EmployeeEdu final : public Visitable<EmployeeEdu>, AutoUID<EmployeeEdu, 'E'> {
    MyString m_uid;
    MyString m_company;
    MyString m_position;

    public:
        EmployeeEdu(const MyString &name, std::istream &input);

        EmployeeEdu(const MyString &uid, const MyString &name, std::istream &input);


        const MyString& uid() const override;

        const MyString& company() const;

        const MyString& position() const;


        void read_details(std::istream &input) override;

        void write(std::ostream &output) const override;

        void release_uid() const override;
};

#endif //EMPLOYEEEDU_H
