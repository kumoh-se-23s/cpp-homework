//
// Created by Merutilm on 10/28/26.
//

#include "MyString.h"
#include <iostream>
#include <fstream>

#include "StudentTable.h"


static Student readStudent(const MyString &line) {
    const int numEndIndex = line.find(" ");
    const MyString numStr = line.substr(0, numEndIndex);
    const int num = stoi(numStr);
    const int groupEndIndex = line.find(" ", numEndIndex + 1);
    const MyString groupStr = line.substr(numEndIndex + 2, groupEndIndex - numEndIndex - 2);
    const int groupId = groupStr.isEmpty() ? Student::NO_GROUP : stoi(groupStr);
    MyString name = line.substr(groupEndIndex + 1);
    return Student(num, std::move(name), groupId);
}

int main(const int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "no input file" << std::endl;
        return 1;
    }
    MyString filename = argv[1];
    std::ifstream ifs(filename.getData());
    if (ifs.fail()) {
        std::cout << "cannot open file" << std::endl;
        return 1;
    }

    MyString line;
    StudentTable table{};

    getline(ifs, line, ' '); //min year
    getline(ifs, line, '\n'); //max year
    getline(ifs, line, ' '); //students
    getline(ifs, line, '\n'); //groups
    do {
        getline(ifs, line);
        if (line.length() != 0) {
            Student student = readStudent(line);
            table.addStudent(std::move(student));
        }
    } while (line.length() != 0);

    char input = '\0';
    while (input != 'Q' && input != 'q') {
        std::cout << "input : ";
        std::cin >> input;
        switch (input) {
            case 'L':
            case 'l':
                std::cout << table << std::endl;
                break;
            case 'I':
            case 'i': {
                getline(std::cin, line); //remove \nq
                getline(std::cin, line);
                Student student = readStudent(line);
                table.addStudent(std::move(student));
                break;
            }
            case 'D':
            case 'd': {
                getline(std::cin, line);
                getline(std::cin, line); //remove \n
                int num = stoi(line);
                table.removeStudent(num);
                break;
            }
            case 'Q':
            case 'q': {
                table.exportFile(filename);
                break;
            }
            default:
                break;
        }
    }

    return 0;
}
