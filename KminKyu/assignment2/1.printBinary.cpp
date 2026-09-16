#include<iostream>

using namespace std;

int main() {
    int testCase;

    cin >> testCase;
    

    for (int nowCase = 0; nowCase < testCase; ++nowCase) {
        int number = 0, arraySize = 0;
        bool binaryArray[32];
        cin >> number;

        while (number != 0) {
            if (number % 2 == 1) {
                binaryArray[arraySize++] = 1;
            } else {
                binaryArray[arraySize++] = 0;
            }
            number /= 2;
        }
        for (int nowIndex = arraySize - 1; nowIndex > -1; --nowIndex) {
            cout << binaryArray[nowIndex];
            if (nowIndex % 4 == 0) {
                cout << " ";
            }
        }
        cout << endl;
    }

    return 0;
}