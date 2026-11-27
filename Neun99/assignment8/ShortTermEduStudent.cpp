#include "ShortTermEduStudent.h"

ShortTermEduStudent::ShortTermEduStudent(const MyString &key, const MyString &name, const MyString &course,
                                         const MyString &department, const MyString &tel) : AdStudent(name, course, tel) {
    this->key = key;
    this->department = department;
}

MyString ShortTermEduStudent::toStringForPrint() const {
    MyString result = "[ShortTermEdu] ";
    result += getName() + "(" + key + ") " + getCourse() + " " + department + " " + getTel();
    return result;
}

MyString ShortTermEduStudent::toStringForSave() const {
    MyString result = "ShortTermEdu ";
    result += key + " " + getName() + " " + getCourse() + " " + department + " " + getTel();
    return result;
}

void ShortTermEduStudent::modify() {
    cout << "Current: " << toStringForPrint() << endl;
    cout << "Input Modified Info with Course Dept Tel" << endl;

    MyString newCourse, newTel;
    cin >> newCourse >> department >> newTel;
    setCourse(newCourse);
    setTel(newTel);

    cout << "<UPDATED> " << toStringForPrint() << endl;
}

bool ShortTermEduStudent::operator==(const MyString &key) const {
    return this->key == key || getTel() == key;
}