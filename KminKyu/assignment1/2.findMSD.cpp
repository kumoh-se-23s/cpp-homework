#include<iostream>

using namespace std;

int main() {
    int testCase;
    cin >> testCase;

    long long value;
    int numberCnt;
    for (int caseNum = 0; caseNum < testCase; ++caseNum) {
        cin >> value;
        numberCnt = 1;
        while (value > 10) {
            value /= 10;
            ++numberCnt;
        }
        cout << value << " @ [" << numberCnt << "]" << endl;
    }

    return 0;
}