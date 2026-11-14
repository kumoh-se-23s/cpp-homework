#pragma once
#include "ArrayList.h"
#include "HashTable.h"
#include "MyString.h"
#include "Student.h"
#include "SinglyLinkedList.h"
#include "KeyList.h"

class App {
    HashTable<MyString, SinglyLinkedList<KeyList<MyString, Student>>> groupHashTable = nullptr;
    HashTable<int, SinglyLinkedList<KeyList<int, Student>>> stduentIdHashTable = nullptr;
    ArrayList<Student> studentArrayList;
};


#endif //CPP_HOMEWORK_GIT_APP_H
