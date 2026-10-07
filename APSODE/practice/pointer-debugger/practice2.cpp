//
// Created by apsode on 26. 10. 7..
//
#include <iostream>
using namespace std;

void testFunction1(int val){
    val = 333 ;
    cout << val << endl ;
}

void testFunction2(int &val) {
    val = 333;
    cout << val << endl ;
}

int main()
{
    int intValue = 777;
    testFunction1(intValue);
    cout << intValue << endl ;
    testFunction2(intValue);
    cout << intValue << endl ;
    return 0 ;
}
