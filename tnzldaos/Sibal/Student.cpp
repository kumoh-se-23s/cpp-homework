#pragma once

#include "Student.h"

// getter
string Student::getId() const { return id; }
string Student::getName() const { return name; }
string Student::getTeamName() const { return teamName; }

// setter
void Student::setId(const string& id) { this->id = id; }
void Student::setName(const string& name) { this->name = name; }
void Student::setTeamName(const string& teamName) { this->teamName = teamName; }

strong_ordering Student::operator<=>(const Student& student) const {
    return this->id <=> student.id;
}

bool Student::operator==(const Student& student) const {
    return this->teamName == student.teamName;
}
