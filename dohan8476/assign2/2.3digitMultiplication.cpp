#include <iostream>
using namespace std;

int main() {
    int num1, num2;
    cin >> num1 >> num2;

    const int SIZE = 3;
    int partialProducts[SIZE];

    for (int i = 0; i < SIZE; i++) {
        partialProducts[i] = num1 * (num2 % 10);
        num2 /= 10;
    }

    int sum = 0;

    for (int i = 0; i < SIZE; i++) {
        int scale = 1;

        cout << partialProducts[i] << endl;
        sum += partialProducts[i]*scale;

        scale *= 10; // 1, 10, 100의 자릿수 맞게 곱해서 출력
    }
    cout << sum << endl;
    return 0;
}