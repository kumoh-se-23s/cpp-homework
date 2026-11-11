#include "Student.h"
Student::Student(int newId, MyString newGroup, MyString newName) {
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
    return idToString() + name;
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

//private---------------------------------------
MyString Student::idToString() const{
    //길이 구하기
    int length = 1;
    for (int tempId = id; tempId > 9; tempId /= 10) {
        length++;
    }

    //char 배열 생성 및 넣기
    char* idChar = new char[length + 1];
    for (int tempId = id, idx = length; idx >= 0; idx++) {
        idChar[idx] = tempId %= 10;
        tempId /= 10;
    }
    idChar[length + 1] = '\0';

    //MyString으로 감싼 뒤 배열 delete 후 리턴
    MyString result = MyString(idChar);
    delete[] idChar;
    return result;
}