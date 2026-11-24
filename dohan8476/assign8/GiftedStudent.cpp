#include "GiftedStudent.h"

#include <iostream>

using namespace std;

GiftedStudent::GiftedStudent() = default;

GiftedStudent::~GiftedStudent() = default;

GiftedStudent::GiftedStudent(const MyString &studentID, const MyString &studentName,
                             const MyString &middleSchoolName, const MyString &course, const MyString &phoneNumber,
                             const MyString &parentName, const MyString &parentPhoneNumber)
            : studentID(studentID), middleSchoolName(middleSchoolName),
            parentName(parentName), parentPhoneNumber(parentPhoneNumber) {

        this->studentName = studentName;
        this->course = course;
        this->phoneNumber = phoneNumber;
}

void GiftedStudent::input() {
    cin >> studentID >> studentName >> middleSchoolName >> course >>
            phoneNumber >> parentName >> parentPhoneNumber;
}

void GiftedStudent::modify() {
    cout << "Input Modified Info with School Course Tel ParentName ParentTel\n";
    cin >> middleSchoolName >> course >> phoneNumber >> parentName >> parentPhoneNumber;
}

MyString GiftedStudent::toString() const {
    return MyString("[Gifted] ") + studentName + " (" + studentID + ") " + middleSchoolName + " " +
            course + " " + phoneNumber + " " + parentName + " (" + parentPhoneNumber + ")";
}

MyString GiftedStudent::toFileFormatString() const {
    return MyString("Gifted ") + studentID + " " + studentName + " " + middleSchoolName + " " +
            course + " " + phoneNumber + " " + parentName + " " + parentPhoneNumber;
}