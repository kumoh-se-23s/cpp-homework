#pragma once
#include "AdStudent.h"

class EmployeeEduStudent : public AdStudent {
public:
    EmployeeEduStudent(const MyString& key, const MyString& name, const MyString& course,
        const MyString& position, const MyString& companyName, const MyString& tel);

    MyString toStringForPrint() const override;
    MyString toStringForSave() const override;
    void modify() override;
    bool operator==(const MyString& key) const override;
private:
    MyString key;
    MyString companyName;
    MyString position;
};
