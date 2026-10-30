//
// Created by Merutilm on 10/28/26.
//

#include "Student.h"

Student::Student(const int num, MyString string, const int teamId): num(num), name(std::move(string)), teamId(teamId) {
}

bool Student::operator==(const Student &student) const {
    return student.num == num;
}

int Student::getNum() const {
    return num;
}

int Student::getYear() const {
    return num / 10000;
}

int Student::getTeamId() const {
    return teamId;
}

const MyString & Student::getName() const {
    return name;
}


MyString Student::toString() const {
    MyString result;
    result.append(MyString::toString(num));
    result.append(" ");
    result.append(name.getData());
    return result;
}


