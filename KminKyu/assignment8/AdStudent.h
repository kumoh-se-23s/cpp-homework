#pragma once
#include <iostream>
#include "MyString.h"

class AdStudent {
    MyString studentPhoneNumber;
    MyString studentName;
    MyString course;

public:
    AdStudent(AdStudent &&other) noexcept
        : studentPhoneNumber(std::move(other.studentPhoneNumber)),
        studentName(std::move(other.studentName)),
        course(std::move(other.course)) {
    }

    virtual ~AdStudent() = 0;

    AdStudent &operator=(AdStudent &&other) noexcept;

    const MyString &getStudentPhoneNumber() const;

    const MyString &getStudentName() const;

    bool operator==(const AdStudent& other) const;
    bool operator!=(const AdStudent& other)const;
    bool operator>(const AdStudent& other) const;
    bool operator<(const AdStudent& other) const;
    bool operator<=(const AdStudent& other) const;
    bool operator>=(const AdStudent& other) const;

    virtual bool operator==(const MyString& studentKey) const;

    virtual bool operator!=(const MyString& studentKey) const;

    //파일로 내보낼때 사용하는 메소드
    virtual void toFile(std::ofstream& out) const = 0;

    //콘솔 출력시 사용하는 메소드
    virtual void print(std::ostream& out) const = 0;

private:
    int compareTo(const AdStudent& other) const;
};
//콘솔 출력시 사용하는 메소드
std::ostream& operator<<(std::ostream& out, const AdStudent& student);

//파일 write시 사용하는 메소드
std::ofstream& operator<<(std::ofstream& out, const AdStudent& student);
