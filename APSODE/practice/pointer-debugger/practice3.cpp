//
// Created by apsode on 26. 10. 7..
//
#include <iostream>
using namespace std;

void testFunction(int arr[]) {
    arr[0] = 77;
}

int main()
{
    const int CAPACITY = 10;
    int intArr[CAPACITY] = {};
    testFunction(intArr);
    cout << intArr[0] << endl;
    return 0 ;
}