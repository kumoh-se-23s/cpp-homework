#include "ShortTermEduStudent.h"
#include <iostream>

using namespace std;

int ShortTermEduStudent::nextID = 1;

ShortTermEduStudent::ShortTermEduStudent() = default;

ShortTermEduStudent::ShortTermEduStudent(const MyString &studentID, const MyString &studentName,
    const MyString &course, const MyString &offerDepartment, const MyString &phoneNumber)
        :studentID(studentID), offerDepartment(offerDepartment){
    this->studentName = studentName;
    this->course = course;
    this->phoneNumber = phoneNumber;

    //nextID 동기화
    int num = stoi(studentID, 1);
    if (num >= nextID) {
        nextID = num + 1;
    }
}

ShortTermEduStudent::~ShortTermEduStudent() = default;

MyString ShortTermEduStudent::generateID() {
    MyString numStr = MyString::toString(nextID++);
    MyString res("S"); 
    for (int i = numStr.length(); i < 5; ++i) {
        res.append('0');
    }
    res.append(numStr);
    return res;
}

const MyString &ShortTermEduStudent::getID() const {
    return studentID;
}

void ShortTermEduStudent::input() {
    studentID = generateID();
    cin >> studentName >> course >> offerDepartment >> phoneNumber;
}

void ShortTermEduStudent::modify() {
    cout << "Input Modified Info with Course Dept Tel\n";
    cin >> course >> offerDepartment >> phoneNumber;
}

MyString ShortTermEduStudent::toString() const {
    return MyString("[ShortTermEdu] ") + studentName + " (" + studentID + ") " +
           course + " " + offerDepartment + " " + phoneNumber;
}

MyString ShortTermEduStudent::toFileFormatString() const {
    return MyString("ShortTermEdu ") + studentID + " " + studentName + " " +
           course + " " + offerDepartment + " " + phoneNumber;
}
