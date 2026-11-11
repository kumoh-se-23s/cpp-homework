//
// Created by Merutilm on 11/11/26.
//

#include "University.h"

bool University::insert(RAIIUniquePointer<AdStudent> v) {
    auto studentIt = students.begin();
    bool inserted = false;
    for (; studentIt.hasNext(); ++studentIt) {
        const MyString &key = (*studentIt)->data->key;
        if (key == v->key) {
            return false;
        }
        if (key > v->key) {
            //key not found
            RAIISharedPointer<DoubleNode<AdStudent> > node = DoubleNode(std::move(v));
            students.insertBefore(*studentIt, node);
            keyToStudent.put(node->data->key, node);
            phoneNumberToStudent.put(node->data->phone, node);

            inserted = true;
            return true;
        }
    }
    if (!inserted) {
        RAIISharedPointer<DoubleNode<AdStudent> > node = DoubleNode(std::move(v));
        students.addLast(node);
        keyToStudent.put(node->data->key, node);
        phoneNumberToStudent.put(node->data->phone, node);
        return true;
    }
    throw std::logic_error("insertion error");
}

void University::remove(const MyString &idOrPhone) {
    if (keyToStudent.containsKey(idOrPhone)) {
        students.remove(*keyToStudent[idOrPhone]);
        keyToStudent.remove(idOrPhone);
    } else if (phoneNumberToStudent.containsKey(idOrPhone)) {
        students.remove(*phoneNumberToStudent[idOrPhone]);
        phoneNumberToStudent.remove(idOrPhone);
    }
}

MyArray<AdStudent *> University::searchCourse(const MyString &course) const {
    MyArray<AdStudent *> result;
    for (auto studentIt = students.begin(); studentIt.hasNext(); ++studentIt) {
        if ((*studentIt)->data->course.find(course) != -1) {
            result.add(&*(*studentIt)->data);
        }
    }
    return result;
}

MyString University::toString() const {
    MyString result;

    for (auto it = students.begin(); it.hasNext(); ++it) {
        result.append((*it)->data->toString()).append("\n");
    }
    return result;
}

MyString University::toFileFormattedString() const {
    MyString result;

    for (auto it = students.begin(); it.hasNext(); ++it) {
        result.append((*it)->data->toFileFormattedString()).append("\n");
    }
    return result;
}

AdStudent *University::find(const MyString &idOrPhone) {
    auto ptr = keyToStudent[idOrPhone];

    if (!ptr) {
        ptr = phoneNumberToStudent[idOrPhone];
    }
    return ptr ? (*ptr)->data.get() : nullptr;
}

void University::update(AdStudent *ptr) {
    const auto tmpPtr = keyToStudent[ptr->key];
    if (!tmpPtr) throw std::logic_error("invalid key");
    const auto tmp = *tmpPtr;
    phoneNumberToStudent.remove(ptr->phone);
    keyToStudent.remove(ptr->key);
    ptr->input();
    keyToStudent.put(ptr->key, tmp);
    phoneNumberToStudent.put(ptr->phone, tmp);
}
