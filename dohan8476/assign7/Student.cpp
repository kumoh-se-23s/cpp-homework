#include "Student.h"

int Student::getStudentID() const {
    return this->studentID;
}

MyString Student::getStudentName() {
    return this->studentName;
}

MyString Student::getTeamID() {
    return this->teamID;
}

void Student::setStudentID(int studentID) {
    this->studentID = studentID;
}

void Student::setStudentName(MyString studentName) {
    this->studentName = studentName;
}

void Student::setTeamID(MyString teamID) {
    this->teamID = teamID;
}

