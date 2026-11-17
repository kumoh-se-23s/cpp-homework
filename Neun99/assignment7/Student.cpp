#include "Student.h"

Student::Student() {
    id = -1;
}

Student::Student(int newId, const MyString &newGroup, const MyString &newName) {
    id = newId;
    group = newGroup;
    name = newName;
}

//getter----------------------------------
int Student::getId() const {
    return id;
}

const MyString& Student::getName() const {
    return name;
}

const MyString& Student::getGroup() const {
    return group;
}

MyString Student::toString() const {
    return itos(id) + " " + name;
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