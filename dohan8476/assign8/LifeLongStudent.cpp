#include "LifeLongStudent.h"
#include <iostream>

using namespace std;

LifeLongStudent::LifeLongStudent() = default;

LifeLongStudent::LifeLongStudent(const MyString &studentID, const MyString &studentName,
    const MyString &course, int fee, const MyString &phoneNumber)
        :studentID(studentID), fee(fee){
    this->studentName = studentName;
    this->course = course;
    this->phoneNumber = phoneNumber;
}

LifeLongStudent::~LifeLongStudent() = default;

const MyString &LifeLongStudent::getID() const {
    return this->studentID;
}

void LifeLongStudent::input() {
    cin >> studentID >> studentName >> course >> fee >> phoneNumber;
}

void LifeLongStudent::modify() {
    cout << "Input Modified Info with Course Amount Tel\n";
    cin >> course >> fee >> phoneNumber;
}

MyString LifeLongStudent::toString() const {
    return MyString("[LifeLong] ") + studentName + " (" + studentID + ") " +
       course + " " + MyString::toString(fee) + " " + phoneNumber;
}

MyString LifeLongStudent::toFileFormatString() const {
    return MyString("LifeLong ") + studentID + " " + studentName + " " +
           course + " " + MyString::toString(fee) + " " + phoneNumber;
}
