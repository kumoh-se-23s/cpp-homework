#include <iostream>
#include "MyString.h"
using namespace std;

int main(){


    MyString str1, str2, str3 = "test", str4 = MyString("tes1111111testdsfdsf"), str5, str6;
    // cin >> str1;
    // cin >> str5;
    // getline(cin, str6);
    str2= str3;
    str2 = str2 + str1;
    str2 = str2 + "1";

    cout << str1 << endl;
    cout << str2 << endl;
    cout << str3 << endl;
    cout << str4 << endl;
    cout << str5 << endl;
    cout << str6 << endl;
    cout << str4.find(str3) << endl;
    cout << str1 + str2 << endl;
    cout << str2 + "dgfg" << endl;
    cout << (str2 == str1) << endl;

    MyString temp1 = "test";
    MyString temp2 = "test";
    cout << (temp1 == temp2) << endl;
    MyString stoi1 = "123axyz";
    MyString stoi2 = "123ab";
    MyString stoi3 = "-123abxyz";
    MyString stoi4 = "123ab456";
    MyString stoi5 = "a";

    cout << stoi(stoi1, 3, 16) << endl;
    cout << stoi(stoi2, 3, 16) << endl;
    cout << stoi(stoi3) << endl;
    cout << stoi(stoi4, 0, 10) << endl;
    cout << stoi(stoi5, 4, 36) << endl;


    MyString insert = "test";
    cout << insert.insert(2, "temp") << endl;
    MyString replace = "test";
    cout << replace.replace(1, 2, "ab") << endl;
    MyString erase = "teerasest";
    cout << erase.erase(2, 5) << endl;

    MyString big = "ZzZZ";
    MyString small = "zZzZZ";
    cout << (big > small) << endl;

}
