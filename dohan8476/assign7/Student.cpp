#include "Student.h"

MyString Student::getStudentID() {
    return this->studentID;
}

MyString Student::getStudentName() {
    return this->studentName;
}

MyString Student::getTeamID() {
    return this->teamID;
}

void Student::setStudentID(MyString studentID) {
    this->studentID = studentID;
}

void Student::setStudentName(MyString studentName) {
    this->studentName = studentName;
}

void Student::setTeamID(MyString teamID) {
    this->teamID = teamID;
}

