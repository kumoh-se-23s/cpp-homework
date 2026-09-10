#include<iostream>

using namespace std;

int main() {
    int testCase;

    long long divValue;
    int maxNum;
    bool isPrime, isFirst;
    cin >> testCase;
    for (int caseNum = 0; caseNum < testCase; ++caseNum) {
        cin >> maxNum;
        for (int nowNum = 2; nowNum < maxNum; ++nowNum) {
            divValue = 0;
            for (int checkDiv = 1; checkDiv <= nowNum/2; ++checkDiv) {
                if (nowNum % checkDiv == 0) {
                    divValue += checkDiv;
                }
            }

            if (divValue == nowNum) {
                cout << "[C] " << nowNum;
                isFirst = true;
                for (int checkDiv = 1; checkDiv <= nowNum/2; ++checkDiv) {
                    if (nowNum % checkDiv == 0) {
                        if (isFirst) {
                            cout << " = ";
                            isFirst = false;
                        } else {
                            cout << " + ";
                        }
                        cout << checkDiv;
                    }
                }
            }

            if (divValue == 1 && nowNum >= 10) {
                if (isPrime) {
                    cout << "\n[P]";
                    isPrime = false;
                }
                cout << nowNum << " ";
            }
        }


    }

}