#include "Student.h";

string Student::getAdmissionYear() const {
    return studentId.substr(0, 4);
}

Student::Student() = default;

Student::Student(const string &studentId, const string &name, const string &teamNum) {
    this->studentId = studentId;
    this->name = name;
    this->teamNum = teamNum;
}

string Student::getStudentId() const {
    return studentId;
};

void Student::setStudentId(const string &studentId) {
    this->studentId = studentId;
}

string Student::getName() const {
    return name;
}

void Student::setName(const string &name) {
    this->name = name;
}

string Student::getTeamNum() const {
    return teamNum;
}

void Student::setTeamNum(const string &teamNum) {
    this->teamNum = teamNum;
}

bool Student::operator==(const Student &student) const {
    return this->teamNum == student.teamNum;
}

bool Student::operator==(const string& studentId) const {
    string admissionYear = getAdmissionYear();
    return admissionYear == studentId;
}