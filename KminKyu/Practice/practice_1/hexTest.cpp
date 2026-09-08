#include<iostream>
using namespace std;

int main() {
    int num = 1237;
    cout << num << endl;
    cout << oct << num << endl;
    cout << hex << num << endl;

    cout << showbase << oct << num << endl;
    cout << showbase << hex << num << endl;

    num = 10; cout << dec << num << endl;
    num = 010; cout << num << endl;
    num = 0x10; cout << num << endl;
}