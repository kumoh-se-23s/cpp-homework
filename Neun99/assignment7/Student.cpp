#include "Student.h"

Student::Student(int newId, MyString newName) {
    id = newId;
    name = newName;
    group = NO_GROUP;
}

Student::Student(int newId, MyString newName, MyString newGroup) {
    id = newId;
    name = newName;
    group = newGroup;
}

//getter----------------------------------
int Student::getId() const {
    return id;
}

MyString Student::getName() const {
    return name;
}

MyString Student::getGroup() const {
    return group;
}

//setter----------------------------------
void Student::setId(int newId) {
    id = newId;
}

void Student::setName(MyString newName) {
    name = newName;
}

void Student::setGroup(MyString newGroup) {
    group = newGroup;
}

//연산자 오버로딩---------------------------------
Student &Student::operator=(const Student &newStudent) {
    if (this != &newStudent) {
        id = newStudent.id;
        name = newStudent.name;
        group = newStudent.group;
    }
    return *this;
}

