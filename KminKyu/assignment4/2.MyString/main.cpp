#include<iostream>
#include"MyString.h"
using namespace std;

int main(){
    OOldMyString str1, str2, str3 = "test", str4 = OOldMyString("tes1111111testdsfdsf"), str5, str6;
    cin >> str1;
    cin >> str5;
    getline(cin, str6);
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

    OOldMyString temp1 = "test";
    OOldMyString temp2 = "test";
    cout << (temp1 == temp2) << endl;

}
