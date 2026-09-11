#include<iostream>

using namespace std;

int main() {
    int testCase;
    long long divValue;
    int maxNum;
    bool isFirstPrime, isFirstMeasure;
    cin >> testCase;
    for (int caseNum = 0; caseNum < testCase; ++caseNum) {
        isFirstPrime = true; isFirstMeasure = true;
        cin >> maxNum;
        for (int nowNum = 2; nowNum <= maxNum; ++nowNum) {
            divValue = 0;
            for (int checkDiv = 1; checkDiv <= nowNum/2; ++checkDiv) {
                if (nowNum % checkDiv == 0) {
                    divValue += checkDiv;
                }
            }
            if (divValue == nowNum) {
                if (!isFirstPrime) {
                    cout << endl;
                }
                cout << "[C] " << nowNum;
                isFirstPrime = true;
                isFirstMeasure = true;
                for (int checkDiv = 1; checkDiv <= nowNum/2; ++checkDiv) {
                    if (nowNum % checkDiv == 0) {
                        if (isFirstMeasure) {
                            cout << " = ";
                            isFirstMeasure = false;
                        } else {
                            cout << " + ";
                        }
                        cout << checkDiv;
                    }
                }
                cout << endl;
            }

            if (divValue == 1 && nowNum >= 10) {
                if (isFirstPrime) {
                    cout << "[P]";
                    isFirstPrime = false;
                }
                cout << nowNum << " ";
            }
        }
        if (!isFirstPrime) {
            cout << endl;
        }


    }

}