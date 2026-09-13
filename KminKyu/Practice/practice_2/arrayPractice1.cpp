#include <iostream>

using namespace std;

void funct1(int &n) {
    n = 999;
}

void funct2(int list[]) {
    list[0] = 999;
}

int * funct3() {
    int arr[10] = {1,1,1,1,1,1,1,1,1,1};
    return arr;
}

int main() {
    const int ARR_MAX = 10;
    int arr[ARR_MAX] = {};

    funct1(arr[0]);
    cout << arr[0] << endl;
    funct2(arr);
    cout << arr[0] << endl;
    return 0;
}
