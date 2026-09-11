#include <iostream>

using namespace std;

int main() {
    int runCntGoal; //목표 동작 횟수
    cin >> runCntGoal;

    for(int runCnt = 0; runCnt < runCntGoal; ++runCnt) {
        double userInput; //유저 입력
        cin >> userInput;

        cout << (int)userInput << " + " << userInput - (int)userInput << endl;
    }

    return 0;
}