#include "LifeLongStudent.h"

LifeLongStudent::LifeLongStudent(const MyString &key, const MyString &name,
                                 const MyString &course, int bill,
                                 const MyString &tel) : AdStudent(name, course, tel) {
    this->key = key;
    this->bill = bill;
}

MyString LifeLongStudent::toStringForPrint() const {
    MyString result = "[LifeLong] ";
    result += getName() + "(" + key + ")" + getCourse() + " " + itos(bill) + " " + getTel();
    return result;
}

MyString LifeLongStudent::toStringForSave() const {
    MyString result = "LifeLong: ";
    result += key + " " + getName() + " " + getCourse() + " " + itos(bill) + " " + getTel();
    return result;
}

void LifeLongStudent::modify() {
    cout << "Current: " << toStringForPrint() << endl;
    cout << "Input Modified Info with Course Amount Tel" << endl;

    MyString newCourse, newTel;
    cin >> newCourse >> bill >> newTel;
    setCourse(newCourse);
    setTel(newTel);

    cout << "<UPDATE> " << toStringForPrint() << endl;
}

bool LifeLongStudent::operator==(const MyString &key) const {
    return this->key == key || getTel() == key;
}
