#include <iostream>

int main()
{
    using namespace std;
    const int MAX = 5;
    int arr[MAX] = {1, 2, 3, 4, 5};
    for (int* p = arr; p < arr + MAX; p++)
        cout << *p << " ";
        cout << endl;
    return 0;
}