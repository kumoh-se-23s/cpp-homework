#include <iostream>

using namespace std;

int main() {
    int runCntGoal; //동작 목표 횟수

    cin >> runCntGoal;

    for(int runCnt = 0; runCnt < runCntGoal; runCnt++) {
        int length; //한 변 길이
        cin >> length;

        //상단부
        for(int idx = 0; idx < length - 1; ++idx) {
            for(int space = 0; space < length - (idx + 1); ++space) {
                cout << " ";
            }
            for(int star = 0; star < length + idx * 2; ++star) {
                cout << "*";
            }
            cout << endl;
        }

        //중단부
        for(int idx = 0; idx < length; ++idx) {
            for(int star = 0; star < length*3 - 2; ++star) {
                cout << "*";
            }
            cout << endl;
        }

        //하단부
        for(int idx = length - 2; idx >= 0; --idx) {
            for(int space = 0; space < length - (idx + 1); ++space) {
                cout << " ";
            }
            for(int star = 0; star < length + idx * 2; ++star) {
                cout << "*";
            }
            cout << endl;
        }
    }
    return 0;
}