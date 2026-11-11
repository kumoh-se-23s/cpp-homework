//
// Created by Merutilm on 11/11/26.
//

#pragma once
#include "AdStudent.h"
#include "CircularLinkedList.h"
#include "MyString.h"
#include "GenericCircularLinkedList.h"
#include "HashMap.h"
#include "StringHasher.h"
#include "MyArray.hpp"

class University {

    HashMap<MyString, RAIISharedPointer<DoubleNode<AdStudent>>, StringHasher> keyToStudent;
    HashMap<MyString, RAIISharedPointer<DoubleNode<AdStudent>>, StringHasher> phoneNumberToStudent;
    MyList<AdStudent> students;

public:

    bool insert(RAIIUniquePointer<AdStudent> v);

    void remove(const MyString & idOrPhone);

    [[nodiscard]] MyArray<AdStudent *> searchCourse(const MyString &course) const;

    [[nodiscard]] MyString toString() const;

    [[nodiscard]] MyString toFileFormattedString() const;

    [[nodiscard]] AdStudent * find(const MyString & idOrPhone);

    void update(AdStudent * ptr);
};
