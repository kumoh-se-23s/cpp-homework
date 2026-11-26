#pragma once
#include "AdStudent.h"

class Gifted : public AdStudent {
    MyString studentId;
    MyString middleSchoolName;
    MyString parentName;
    MyString parentPhoneNumber;

public:

    Gifted(const MyArrayList<MyString>& input);

    ~Gifted() override = default;

    const MyString& getStudentId() const;

    const MyString& getMiddleSchoolName() const;

    const MyString& getParentName() const;

    const MyString& getParentPhoneNumber() const;

    bool operator==(const MyString &studentKey) const override;

    bool operator!=(const MyString &studentKey) const override;

    MyString toFileString() const override;

    MyString toString() const override;

    void update(const MyArrayList<MyString> &input) override;
};



