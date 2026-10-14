#include <iostream>
#include "MyString.h"
using namespace std;

ostream& operator<<(ostream& os, const MyString &str) {
    for(int index = 0 ; index < str.length() ; index++) {
        os << str.at(index);
    }

    return os;
}

istream& operator>>(istream& is, MyString &str) {
    int index{0};
    char inputChar{};

    if(is.get(inputChar)) {
        while(inputChar != ' ' && index < 15) {
            str.setChar(inputChar, index);

            index++;
        }
    }
    return is;
}

istream& getline(istream& is, MyString &str, char delim='\n') {
    int index{0};
    char inputChar{};

    if(is.get(inputChar)) {
        while(inputChar!= delim && index < 15) {
            str.setChar(inputChar, index);

            index++;
        }
    }
    return is;
}