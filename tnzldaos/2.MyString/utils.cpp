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

    int index{0};
    char inputChar{};

    while (is.get(inputChar) && utils.isCharacter(inputChar) && index < 15) {
        str.appendChar(inputChar);
        index++;
    }

    return is;
}

istream& getline(istream& is, MyString &str, char delim='\n') {
    Utils utils;

    int index{0};
    char inputChar{};

    while (is.get(inputChar) && inputChar != delim && index < 15) {
        str.appendChar(inputChar);
        index++;
    }
    return is;
}