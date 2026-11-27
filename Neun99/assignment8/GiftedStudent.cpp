#include "GiftedStudent.h"

GiftedStudent::GiftedStudent(const MyString &key, const MyString &name, const MyString &middleSchoolName,
                             const MyString &course, const MyString &tel,
                             const MyString &parentName, const MyString &parentTel) : AdStudent(name, course, tel) {
    this->key = key;
    this->middleSchoolName = middleSchoolName;
    this->parentName = parentName;
    this->parentTel = parentTel;
}

MyString GiftedStudent::toStringForPrint() const {
    MyString result = "[Gifted] ";
    result += getName() + "(" + key + ") " + middleSchoolName + " " + getCourse() + " " + getTel() + " ";
    result += parentName + "(" + parentTel + ")\n";
    return result;
}

MyString GiftedStudent::toStringForSave() const {
    MyString result = "Gifted ";
    result += key + " " + getName() + " " + middleSchoolName + " " + getCourse() + " " + getTel() + " ";
    result += parentName + " " + parentTel;
    return result;
}

void GiftedStudent::modify() {
    cout << "Current: " << toStringForPrint() << endl;
    cout << "Input Modified Info with School Course Tel ParentName ParentTel" << endl;

    MyString newCourse, newTel;
    cin >> middleSchoolName >> newCourse >> newTel >> parentName >> parentTel;
    setCourse(newCourse);
    setTel(newTel);

    cout << "<UPDATE> " << toStringForPrint() << endl;
}

bool GiftedStudent::operator==(const MyString &key) const {
    return this->key == key || getTel() == key;
}
