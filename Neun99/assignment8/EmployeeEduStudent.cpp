#include "EmployeeEduStudent.h"

EmployeeEduStudent::EmployeeEduStudent(const MyString &key, const MyString &name,
                                       const MyString &companyName, const MyString &position,
                                       const MyString &course, const MyString &tel): AdStudent(name, course, tel) {
    this->key = key;
    this->position = position;
    this->companyName = companyName;
}

MyString EmployeeEduStudent::toStringForPrint() const {
    MyString result = "[EmployeeEdu] ";
    result += getName() + "(" + key + ") " + companyName + " " + position + " " + getCourse() + " " + getTel();
    return result;
}

MyString EmployeeEduStudent::toStringForSave() const {
    MyString result = "EmployeeEdu ";
    result += key + " " + getName() + " " + companyName + " " + position + " " + getCourse() + " " + getTel();
    return result;
}

void EmployeeEduStudent::modify() {
    cout << "Current: " << toStringForPrint() << endl;
    cout << "Input Modified Info with Company Position Course Tel" << endl;

    MyString newCourse, newTel;
    cin >> companyName >> position >> newCourse >> newTel;
    setCourse(newCourse);
    setTel(newTel);

    cout << "<UPDATE> " << toStringForPrint() << endl;
}

bool EmployeeEduStudent::operator==(const MyString &key) const {
    return this->key == key || getTel() == key;
}