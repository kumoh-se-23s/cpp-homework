//
// Created by apsode on 26. 10. 7..
//
#include <iostream>
using namespace std;

int main(){
    const int CAPACITY = 10;
    int arr[CAPACITY] = { 1,2,3,4,5,6,7,8,9,10 };

    int tmpArr[CAPACITY];
    tmpArr = arr;

    int* tmpPArr = new int[CAPACITY];
    tmpPArr = arr;

    for (int* ptr = arr; ptr < &arr[10]; ptr++) {
        cout << ptr << " : " << *ptr << endl;
    }

    return 0;
}