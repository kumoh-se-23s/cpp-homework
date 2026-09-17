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

    for (int i = 0, scale = 1; i < SIZE; i++, scale *= 10) {
        cout << partialProducts[i] << endl;
        sum += partialProducts[i]*scale;
    }
    cout << sum << endl;
    return 0;
}