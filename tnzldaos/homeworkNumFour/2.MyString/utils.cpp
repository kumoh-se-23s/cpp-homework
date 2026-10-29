#include <iostream>
#include "utils.h"
#include "MyString.h"
using namespace std;

bool Utils::isCharacter(char input) {
    return (32 < input) & (input < 127);
}

int Utils::getCharArrLength(const char inputCharArr[]) {
    int index{0};

    while (inputCharArr[index] != '\0') {
        index++;
    }
    
    return index;
}

ostream& operator<<(ostream& os, const MyString &str) {
    for(int index = 0 ; index < str.length() ; index++) {
        os << str.at(index);
    }

    return os;
}

istream& operator>>(istream& is, MyString &str) {
    Utils utils;

    bool isStart{false};
    int index{0};
    char inputChar{'\0'};

    while (index < 15) {
        inputChar = is.peek();

        if(utils.isCharacter(inputChar)) {
            is.get(inputChar);
            str.setChar(inputChar, index);
            isStart = true;
            index++;
        }
        else if (isStart) {
            break;
        }
        else {
            is.get();
        }
    }
    str.setChar('\0', index);
    str.setLength(index);

    return is;
}

istream& getline(istream& is, MyString &str, char delim='\n') {
    Utils utils;

    int index{0};
    char inputChar{'\0'};

    while (index < 15 && is.get(inputChar) && inputChar != delim) {
        str.setChar(inputChar, index);
        index++;
    }

    str.setChar('\0', index);
    str.setLength(index);

    if(index == 15) {
        while (is.get(inputChar) && inputChar != delim) {

        }
    }

    return is;
}