//
// Created by Merutilm on 10/28/26.
//

#include "MyString.h"
#include <iostream>
#include <fstream>

#include "StudentHashTable.h"


static Student readStudent(const MyString &line) {
    const int numEndIndex = line.find(" ");
    const MyString numStr = line.substr(0, numEndIndex);
    const int num = MyString::stoi(numStr);
    const int groupEndIndex = line.find(" ");
    const MyString groupStr = line.substr(numEndIndex + 2, groupEndIndex);
    const int groupId = groupStr == "-" ? Student::NO_GROUP : MyString::stoi(groupStr);
    MyString name = line.substr(groupEndIndex + 1);
    return Student(num, std::move(name), groupId);
}

int main(const int argc, char *argv[]) {
    if (argc != 1) {
        std::cout << "no input file" << std::endl;
        return 1;
    }
    MyString filename = argv[0];
    std::ifstream ifs(filename.getData());
    if (ifs.fail()) {
        std::cout << "cannot open file" << std::endl;
        return 1;
    }

    MyString line;
    StudentHashTable table{};
    do {
        MyString::getline(ifs, line);
        if (line.length() != 0) {
            Student student = readStudent(line);
            table.addStudent(std::move(student));
        }
    } while (line.length() != 0);

    char input = '\0';
    while (input != 'Q' && input != 'q') {
        std::cin >> input;
        switch (input) {
            case 'L':
            case 'l':
                std::cout << table << std::endl;
                break;
            case 'I':
            case 'i': {
                MyString::getline(std::cin, line);
                Student student = readStudent(line);
                table.addStudent(std::move(student));
                break;
            }
            case 'D':
            case 'd': {
                MyString::getline(std::cin, line);
                Student student = readStudent(line);
                table.removeStudent(student);
                break;
            }
            default:
                break;
        }
    }

    return 0;
}
