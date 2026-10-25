#include <iostream>
#include "MyString.h"

using namespace std;

int main() {
    MyString str1;
    MyString str2;
    MyString str3;
    cin >> str1;
    cin >> str2;
    getline(cin, str3);
    cout << "1: " << str1 << endl;
    cout << "2: " << str2 << endl;
    cout << "3: " << str3 << endl << endl;

    // MyString str4;
    // str4 = str1;
    // cout << str4 << endl;
    // str4 = str1 + str2;
    // cout << str4 << endl;
    // cout << (str1 == str2) << endl;
    // cout << str1[3] << endl;

    return 0;
}