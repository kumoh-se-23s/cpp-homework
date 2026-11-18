#include "Student.h"

Student::Student() : studentID(0), studentName(""), teamID("-"){}

Student::Student(int studentID, const MyString& studentName, const MyString& teamID)
    : studentID(studentID), studentName(studentName), teamID(teamID) {}

int Student::getStudentID() const {
    return this->studentID;
}

const MyString& Student::getStudentName() const{
    return this->studentName;
}

const MyString& Student::getTeamID() const{
    return this->teamID;
}

void Student::setStudentID(int studentID) {
    this->studentID = studentID;
}

void Student::setStudentName(const MyString& studentName) {
    this->studentName = studentName;
}

void Student::setTeamID(const MyString& teamID) {
    this->teamID = teamID;
}

int Student::getYear() const {
    return studentID/10000;
}

bool Student::hasTeam() const {
    return teamID != "" || teamID != "-";
}

bool Student::operator<(const Student &other) const {
    return this->studentID < other.studentID;
}

bool Student::operator==(const Student &other) const {
    return this->studentID == other.studentID;
}

bool Student::operator==(int otherID) const {
    return this->studentID == otherID;
}
