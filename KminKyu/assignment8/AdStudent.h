//
// Created by lhetl on 2026-11-22.
//

#ifndef CPP_HOMEWORK_GIT_ADSTUDENT_H
#define CPP_HOMEWORK_GIT_ADSTUDENT_H
#include <MyString.h>


class AdStudent {
    MyString studentPhoneNumber;
    MyString studentName;
    MyString course;

public:
    AdStudent(const MyString& studentPhoneNumber, const MyString &studentName, const MyString& course)
        : studentPhoneNumber(studentPhoneNumber),
        studentName(studentName),
        course(course) {}

    virtual ~AdStudent() = 0;

    AdStudent(const AdStudent &other)
        : studentPhoneNumber(other.studentPhoneNumber),
        studentName(other.studentName),
        course(other.course) {
    }

    AdStudent(AdStudent &&other) noexcept
        : studentPhoneNumber(std::move(other.studentPhoneNumber)),
        studentName(std::move(other.studentName)),
        course(std::move(other.course)) {
    }

    AdStudent &operator=(const AdStudent &other);

    AdStudent &operator=(AdStudent &&other) noexcept;


    const MyString &getStudentPhoneNumber() const;

    const MyString &getStudentName() const;

    //파일로 내보낼때 사용하는 메소드
    virtual void toFile(std::ofstream& out) const = 0;

    //콘솔 출력시 사용하는 메소드
    virtual void print(std::ostream& out) const = 0;

    //파일, 콘솔 입력시 사용하는 메소드
    virtual bool input(std::istream& in) = 0;

    //수정할 값을 입력받는 메소드
    virtual bool update(std::istream& in) = 0;
};
//콘솔 출력시 사용하는 메소드
std::ostream& operator<<(std::ostream& out, const AdStudent& student);

//콘솔, 파일 입력시 사용하는 메소드
std::istream& operator>>(std::istream& in, AdStudent& student);
std::ofstream& operator<<(std::ofstream& out, const AdStudent& student);
#endif //CPP_HOMEWORK_GIT_ADSTUDENT_H
