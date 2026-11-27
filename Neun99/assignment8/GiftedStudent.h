#pragma once
#include "AdStudent.h"

class GiftedStudent : public AdStudent {
public:
    GiftedStudent(const MyString &key, const MyString &name, const MyString &middleSchoolName,
                  const MyString &course, const MyString &tel, const MyString &parentName, const MyString &parentTel);

    MyString toStringForPrint() const override;
    MyString toStringForSave() const override;
    void modify() override;
    bool operator==(const MyString& key) const override;

private:
    MyString key;
    MyString middleSchoolName;
    MyString parentName;
    MyString parentTel;
};
