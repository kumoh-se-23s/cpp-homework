#pragma once
#include "AdStudent.h"

class EmployeeEdu : public AdStudent {

    MyString studentId;
    MyString companyName;
    MyString position;

    static int LAST_STUDENT_ID;
    static constexpr int MAX_STUDENT_ID_SIZE = 5;

public:

    //콘솔 입력용 생성자
    EmployeeEdu(const MyArrayList<MyString>& input);

    //파일 입력용 생성자
    EmployeeEdu(const MyString &studentId, const MyArrayList<MyString> &input);

    ~EmployeeEdu() override = default;

    const MyString& getStudentId() const;

    const MyString& getCompanyName() const;

    const MyString& getPosition() const;

    MyString toFileString() const override;

    MyString toString() const override;

    bool operator==(const MyString &studentKey) const override;

    bool operator!=(const MyString &studentKey) const override;

    void update(const MyArrayList<MyString> &input) override;

};

int EmployeeEdu::LAST_STUDENT_ID = 0;


