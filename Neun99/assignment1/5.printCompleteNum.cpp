#include <iostream>

using namespace std;

int main() {
    int runCntGoal;

    cin >> runCntGoal;

    for(int runCnt = 0; runCnt < runCntGoal; ++runCnt) {
        int userInput;
        cin >> userInput;

        bool isPLineOpen= false;
        //2부터 input까지의 모든 정수에 대해
        for (int idx = 2; idx <= userInput; ++idx) {
            int idxDivSum = 1;

            //약수 구해서 완전수 및 소수 찾기
            for (int idxDiv = 2; idxDiv < idx; ++idxDiv) {
                if (idx % idxDiv == 0)
                    idxDivSum += idxDiv;
            }

            //완전수 찾으면 출력
            if (idxDivSum == idx) {
                if (isPLineOpen) { //P줄 열려있으면 닫고 C줄 출력
                    cout << endl;
                    isPLineOpen = false;
                }
                printf("[C] %d = 1",idx);
                for (int idxDiv = 2; idxDiv < idx; ++idxDiv) {
                    if (idx % idxDiv == 0)
                        printf(" + %d", idxDiv);
                }
                cout << endl;
            }

            //10 이상의 소수 찾으면 출력
            if (idxDivSum > 1 && idx >= 10) {
                if(!isPLineOpen) { //P줄 닫혀있으면 열고 출력
                    cout << "[P]";
                    isPLineOpen = true;
                }
                printf(" %d", idx);
            }
        }
        //한 회차 동작 끝날 때 P줄이 열려있으면 endl (다음 동작 입력 대비)
        if (isPLineOpen)
            cout << endl;
    }

    return 0;
}