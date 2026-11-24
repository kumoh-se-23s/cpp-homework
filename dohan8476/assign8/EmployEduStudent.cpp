#include "EmployEduStudent.h"
#include <iostream>

#include "../../cmake-build-debug/_deps/googletest-src/googletest/src/gtest-internal-inl.h"

using namespace std;

EmployEduStudent::EmployEduStudent() = default;

EmployEduStudent::EmployEduStudent(const MyString &studentID, const MyString &studentName, const MyString &companyName,
    const MyString &position, const MyString &course, const MyString &phoneNumber)
        : studentID(studentID),companyName(companyName),position(position){
    this->studentName = studentName;
    this->course = course;
    this->phoneNumber = phoneNumber;
}

EmployEduStudent::~EmployEduStudent() = default;

MyString EmployEduStudent::generateID() {
    MyString numStr = MyString::toString(nextID++);
    MyString res("E");
    for (int i = numStr.length(); i < 5; ++i) {
        res.append('0');
    }
    res.append(numStr);
    return res;
}

const MyString &EmployEduStudent::getID() const {
    return studentID;
}

void EmployEduStudent::input() {
    studentID = generateID();
    cin >> studentName >> companyName >> position >> course >> phoneNumber;
}

void EmployEduStudent::modify() {
    cout << "Input Modified Info with Company Position Course Tel\n";
    cin >> companyName >> position >> course >> phoneNumber;
}

MyString EmployEduStudent::toString() const {
    return MyString("[EmployeeEdu] ") + studentName + " (" + studentID + ") " +
           companyName + " " + position + " " + course + " " + phoneNumber;
}

MyString EmployEduStudent::toFileFormatString() const {
    return MyString("EmployeeEdu ") + studentID + " " + studentName + " " +
           companyName + " " + position + " " + course + " " + phoneNumber;
}


