#pragma once
#include "ArrayList.h"
#include "HashTable.h"
#include "MyString.h"
#include "Student.h"
#include "SinglyLinkedList.h"
#include "Pair.h"

class App {
    HashTable<MyString, SinglyLinkedList<Pair<MyString, Student>>> groupHashTable = nullptr;
    HashTable<int, SinglyLinkedList<Pair<int, Student>>> stduentIdHashTable = nullptr;
    ArrayList<Student> studentArrayList;
};


#endif //CPP_HOMEWORK_GIT_APP_H
