#pragma once
#include "SinglyLinkedList.hpp"
#include "AdStudent.h"

class FileIO {
public:
    static bool loadFile(const char* filename, SinglyLinkedList<AdStudent>& list);
    static bool saveFile(const char* filename, const SinglyLinkedList<AdStudent>& list);
};