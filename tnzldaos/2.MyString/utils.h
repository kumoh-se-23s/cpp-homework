#pragma once
#include <iostream>
#include "MyString.h"
using namespace std;

class Utils {
    public:
        bool isCharacter(char input);

};

ostream& operator<<(ostream& os, const MyString &str);
istream& operator>>(istream& is, MyString &str);
istream& getline(istream& is, MyString &str, char delim);