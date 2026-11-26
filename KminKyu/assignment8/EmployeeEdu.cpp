#include "EmployeeEdu.h"

EmployeeEdu::EmployeeEdu(const MyArrayList<MyString> &input) : AdStudent(input.at(0), input.at(3), input.at(4)) {
    this->studentId = "E";

    MyString stringValue = MyString::integerToString(++LAST_STUDENT_ID);

    for (int i = stringValue.length(); i <= MAX_STUDENT_ID_SIZE; ++i) {
        studentId.append("0");
    }

    this->studentId.append(stringValue);

    this->companyName = input.at(1);
    this->position = input.at(2);
}

EmployeeEdu::EmployeeEdu(const MyString &studentId, const MyArrayList<MyString> &input) : AdStudent(input.at(0), input.at(3), input.at(4)) {
    this->studentId = studentId;
    int studentIdValue = stoi(studentId, 1, 10);
    LAST_STUDENT_ID = std::max(studentIdValue, LAST_STUDENT_ID);

    this->companyName = input.at(1);
    this->position = input.at(2);
}

const MyString& EmployeeEdu::getStudentId() const {
    return studentId;
}

const MyString& EmployeeEdu::getCompanyName() const {
    return companyName;
}


const MyString& EmployeeEdu::getPosition() const {
    return position;
}

MyString EmployeeEdu::toFileString() const {
}

MyString EmployeeEdu::toString() const {
}

bool EmployeeEdu::operator==(const MyString &studentKey) const {
    return (AdStudent::operator==(studentKey) || this->studentId == studentKey);
}

bool EmployeeEdu::operator!=(const MyString &studentKey) const {
    return (AdStudent::operator!=(studentKey) || this->studentId != studentKey);
}

void EmployeeEdu::update(const MyArrayList<MyString> &input) {
}
