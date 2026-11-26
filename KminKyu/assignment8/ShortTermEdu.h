#pragma once
#include "AdStudent.h"

class ShortTermEdu : public AdStudent {
    MyString studentId;
    MyString department;

    static int LAST_STUDENT_ID;
    static constexpr int MAX_STUDENT_ID_SIZE = 5;

public:

    //콘솔 입력용 생성자
    ShortTermEdu(const MyArrayList<MyString>& input);

    //파일 입력용 생성자
    ShortTermEdu(const MyString& studentId, const MyArrayList<MyString>& input);

    ~ShortTermEdu() override = default;

    const MyString& getStudentId() const;

    const MyString& getDepartment() const;

    bool operator==(const MyString &studentKey) const override;

    bool operator!=(const MyString &studentKey) const override;

    MyString toFileString() const override;

    MyString toString() const override;

    void update(const MyArrayList<MyString> &input) override;
};

int ShortTermEdu::LAST_STUDENT_ID = 0;



