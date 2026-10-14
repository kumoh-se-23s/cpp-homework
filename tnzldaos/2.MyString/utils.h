#include <iostream>
#include "MyString.h"
using namespace std;

ostream& operator<<(ostream& os, const MyString &str);
istream& operator>>(istream& is, MyString &str);
istream& getline(istream& is, MyString &str, char delim='\n');