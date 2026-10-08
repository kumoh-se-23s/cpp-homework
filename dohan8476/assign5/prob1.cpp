#include <iostream>
using namespace std;

int main()
{
    const int MAX = 5;
    int arr[MAX] = { 1,2,3,4,5 };
    for (int* p = arr ; p < arr + MAX ; p++)
        cout << *p << " "; // 이 줄에서 arr 사용하지 않고 p만 사용
    cout << endl;
    return 0;
}