#include<iostream>

using namespace std;
int main() {
    int testCase;
    cin >> testCase;


    for (int caseNum = 0; caseNum < testCase; ++caseNum) {
        long long value;
        cin >> value;

        int numberCnt = 1;

        while (value / 10 != 0) {
            value /= 10;
            ++numberCnt;
        }
        cout << value << " @ [" << numberCnt << "]" << endl;
    }

    return 0;
}