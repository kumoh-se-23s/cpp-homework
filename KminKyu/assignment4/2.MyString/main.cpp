#include<iostream>
#include"MyString.h"
using namespace std;

int main(){
    MyString str1, str2, str3 = "test", str4 = MyString("tes1111111testdsfdsf");
    cin >> str1;
    str2= str3;
    str2 = str2 + str1;
    str2 = str2 + "1";
    cout << str1 << endl;
    cout << str2 << endl;
    cout << str3 << endl;
    cout << str4 << endl;
    cout << str4.find(str3) << endl;
}
