#include "StudentManageApp.h"
#include "GiftedStudent.h"
#include "LifeLongStudent.h"
#include "EmployeeEduStudent.h"
#include "ShortTermEduStudent.h"

#include "MyIterator.hpp"

void StudentManageApp::run(const char* file) {

}

//private-------------------------
void StudentManageApp::readfile(const char* file) {
    ifstream instream;
    instream.open(file);

    while (!instream.fail()) {
        MyString type;
        instream >> type;
        AdStudent* newStudent = createStudentWithFile(type, instream);
        if (newStudent != nullptr)
            list.addAsc(newStudent);
    }
}

AdStudent *StudentManageApp::createStudentWithFile(const MyString &type, ifstream& in) {
    if (type == GIFTED) {
        MyString key, name, middleSchoolName, course, tel, parentName, parentTel;
        in >> key >> name >> middleSchoolName >> course >> tel >> parentName >> parentTel;
        return new GiftedStudent(key, name, middleSchoolName, course, tel, parentName, parentTel);
    }

    if (type == LIFELONG) {
        MyString key, name, course, tel;
        int bill;
        in >> key >> name >> course >> bill >> tel;
        return new LifeLongStudent(key, name, course, bill, tel);
    }

    if (type == EMPLOYEE_EDU) {
        MyString key, name, course, position, companyName, tel;
        in >> key >> name >> course >> position >> companyName >> tel;
        return new EmployeeEduStudent(key, name, course, position, companyName, tel);
    }

    if (type == SHORT_TERM_EDU) {
        MyString key, name, course, department, tel;
        in >> key >> name >> course >> department >> tel;
        return new ShortTermEduStudent(key, name, course, department, tel);
    }

    return nullptr;
}
