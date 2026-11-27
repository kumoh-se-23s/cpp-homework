#pragma once
#include "MyString.h"

using namespace std;

class AdStudent {
public:
    AdStudent(const MyString &name, const MyString &course, const MyString &tel);
    virtual ~AdStudent();

    //getter
    const MyString &getName() const;
    const MyString &getCourse() const;
    const MyString &getTel() const;

    //setter
    void setName(const MyString &name);
    void setCourse(const MyString &course);
    void setTel(const MyString &tel);

    //기능
    virtual MyString toStringForPrint() const = 0;
    virtual MyString toStringForSave() const = 0;
    virtual void modify() = 0;

    //연산자 오버로딩
    //TODO: 안쓰는거 정리하기
    virtual bool operator==(const MyString& key) const = 0;
    bool operator==(const AdStudent &std2) const;
    bool operator!=(const AdStudent &std2) const;
    bool operator<(const AdStudent &std2) const;
    bool operator<=(const AdStudent &std2) const;
    bool operator>(const AdStudent &std2) const;
    bool operator>=(const AdStudent &std2) const;

private:
    MyString name;
    MyString course;
    MyString tel;
};
