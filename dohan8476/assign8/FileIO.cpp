#include "FileIO.h"
#include "GiftedStudent.h"
#include "LifeLongStudent.h"
#include "EmployEduStudent.h"
#include "ShortTermEduStudent.h"
#include <fstream>

using namespace std;

bool FileIO::loadFile(const char* filename, SinglyLinkedList<AdStudent>& list) {
    ifstream fin(filename);
    if (!fin.is_open()) {
        return false;
    }

    MyString adStudent;
    while (fin >> adStudent) {
        AdStudent* student = nullptr;

        if (adStudent == "Gifted") {
            MyString id, name, school, course, phone, pName, pPhone;
            fin >> id >> name >> school >> course >> phone >> pName >> pPhone;
            student = new GiftedStudent(id, name, school, course, phone, pName, pPhone);
        } else if (adStudent == "LifeLong") {
            MyString id, name, course, phone;
            int fee;
            fin >> id >> name >> course >> fee >> phone;
            student = new LifeLongStudent(id, name, course, fee, phone);
        } else if (adStudent == "EmployeeEdu") {
            MyString id, name, company, pos, course, phone;
            fin >> id >> name >> company >> pos >> course >> phone;
            student = new EmployEduStudent(id, name, company, pos, course, phone);
        } else if (adStudent == "ShortTermEdu") {
            MyString id, name, course, dept, phone;
            fin >> id >> name >> course >> dept >> phone;
            student = new ShortTermEduStudent(id, name, course, dept, phone);
        }

        if (student != nullptr) {
            list.insertAscend(student);
        }
    }

    fin.close();
    return true;
}

bool FileIO::saveFile(const char* filename, const SinglyLinkedList<AdStudent>& list) {
    ofstream fout(filename);
    if (!fout.is_open()) {
        return false;
    }

    SinglyNode<AdStudent>* curr = list.getHead();
    while (curr != nullptr) {
        fout << curr->getData()->toFileFormatString() << endl;
        curr = curr->getNext();
    }

    fout.close();
    return true;
}