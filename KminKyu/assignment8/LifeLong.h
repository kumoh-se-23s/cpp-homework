#pragma once
#include "AdStudent.h"
#include "MyString.h"

class LifeLong : public AdStudent {
    MyString studentId;
    int tutionFee;

public:

    LifeLong(const MyArrayList<MyString>& input);

    ~LifeLong() override = default;

    const MyString& getStudentId() const;

    int getTutionFee() const;

    bool operator==(const MyString &studentKey) const override;

    bool operator!=(const MyString &studentKey) const override;

    MyString toFileString() const override;

    MyString toString() const override;

    void update(const MyArrayList<MyString> &input) override;
};

