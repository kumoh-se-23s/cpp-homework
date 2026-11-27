#pragma once
#include "AdStudent.h"

class LifeLongStudent : public AdStudent {
public:
    LifeLongStudent(const MyString &key, const MyString &name, const MyString &course, int bill, const MyString &tel);
    MyString toStringForPrint() const override;
    MyString toStringForSave() const override;
    void modify() override;
    bool operator==(const MyString& key) const override;

private:
    MyString key;
    int bill;
};
