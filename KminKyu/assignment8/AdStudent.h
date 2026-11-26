#pragma once
#include <iostream>
#include "MyString.h"

class AdStudent {
    MyString studentPhoneNumber;
    MyString studentName;
    MyString course;

public:
    AdStudent(const MyString& studentName, const MyString& course, const MyString& studentPhoneNumber)
        : studentName(studentName),
        course(course),
        studentPhoneNumber(studentPhoneNumber) {}

    AdStudent(AdStudent &&other) noexcept
        : studentName(std::move(other.studentName)),
        course(std::move(other.course)),
        studentPhoneNumber(std::move(other.studentPhoneNumber)) {}

    virtual ~AdStudent() = 0;

    AdStudent &operator=(AdStudent &&other) noexcept;

    const MyString& getStudentPhoneNumber() const;

    const MyString& getStudentName() const;

    const MyString& getCourse() const;

    void setStudentPhoneNumber(const MyString& phoneNumber);

    void setStudentName(const MyString& studentName);

    void setCourse(const MyString& course);

    //정렬용 오버로딩
    bool operator==(const AdStudent& other) const;
    bool operator!=(const AdStudent& other)const;
    bool operator>(const AdStudent& other) const;
    bool operator<(const AdStudent& other) const;
    bool operator<=(const AdStudent& other) const;
    bool operator>=(const AdStudent& other) const;

    //student 키값 비교용 오버로딩
    virtual bool operator==(const MyString& studentKey) const;

    virtual bool operator!=(const MyString& studentKey) const;

    //파일 출력시 사용하는 메소드
    virtual MyString toFileString() const = 0;

    //콘솔 출력시 사용하는 메소드
    virtual MyString toString() const = 0;

    //Modify 연산 시 사용하는 메소드 -> 이미 split 된 MyArrayList를 통해 업데이트
    virtual void update(const MyArrayList<MyString> &input) = 0;

private:
    int compareTo(const AdStudent& other) const;
};

std::ostream& operator<<(std::ostream& out, const AdStudent& student);
