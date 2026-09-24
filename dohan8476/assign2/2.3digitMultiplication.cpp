#include <iostream>
using namespace std;

int main() {
    int num1, num2;
    cin >> num1 >> num2;

    const int SIZE = 3;
    int midResArr[SIZE];

    for (int i = 0; i < SIZE; i++) {
        midResArr[i] = num1 * (num2 % 10);
        num2 /= 10;
    }

    int sum = 0;
    int scale = 1;

    for (int i = 0; i < SIZE; i++) {

        cout << midResArr[i] << endl;
        sum += midResArr[i]*scale;

        scale *= 10; // 1, 10, 100의 자릿수 맞게 곱해서 출력
    }
    cout << sum << endl;
    return 0;
}