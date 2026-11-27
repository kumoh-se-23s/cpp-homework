#pragma once
#include "AdStudent.h"

class ShortTermEduStudent : public AdStudent {
public:
    ShortTermEduStudent(const MyString& key, const MyString& name, const MyString& course, const MyString& department, const MyString& tel);

    MyString toStringForPrint() const override;
    MyString toStringForSave() const override;
    void modify() override;
    bool operator==(const MyString& key) const override;
private:
    MyString key;
    MyString department;
};