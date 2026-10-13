#include <iostream>
#include "MyString.h"

using namespace std;

int main() {
    MyString str1;
    MyString str2;
    MyString str3;
    cin >> str1;
    cin >> str2;
    //cin.getline(str3);
    cout << str1 << endl;
    cout << str2 << endl;
    cout << str3 << endl;

    MyString str4;
    str4 = str1;
    cout << str4 << endl;
    str4 = str1 + str2;
    cout << str4 << endl;
    cout << (str1 == str2) << endl;
    cout << str1[3] << endl;

    return 0;
}