//
// Created by apsode on 26. 10. 7..
//
#include <iostream>

using namespace std;

int main()
{
    int intValue = 777, * iPtr; // 포인터 선언
    iPtr = &intValue;			// 포인터 대입
    *iPtr = 333;				// 포인터가 가리키는 곳의 값 변경
    cout << intValue << endl;
    return 0 ;
}
