#include <iostream>

using namespace std;

int main() {
    int runCntGoal;

    cin >> runCntGoal;

    for(int runCnt = 0; runCnt < runCntGoal; ++runCnt) {
        int userInput;
        cin >> userInput;

        bool isPrintedP = false;
        //2부터 input까지의 모든 정수에 대해
        for (int idx = 2; idx <= userInput; ++idx) {
            int idxDivSum = 1;
            bool isDivFound = false;

            //약수 구해서 완전수 및 소수 찾기
            for (int idxDiv = 2; idxDiv < idx; ++idxDiv) {
                if (idx % idxDiv == 0) {
                    isDivFound = true;
                    idxDivSum += idxDiv;
                }
            }

            //완전수 찾으면 출력
            if (idxDivSum == idx && idxDivSum != 1) {
                if (isPrintedP) {
                    cout << endl;
                    isPrintedP = false;
                }

                printf("[C] %d = 1",idx);
                isPrintedP = false;
                for (int idxDiv = 2; idxDiv < idx; ++idxDiv) {
                    if (idx % idxDiv == 0)
                        printf(" + %d", idxDiv);
                }
                cout << endl;
            }

            //10 이상의 소수 찾으면 출력
            if (!isDivFound && idx >= 10) {
                if(!isPrintedP) {
                    cout << "[P]";
                    isPrintedP = true;
                }
                printf(" %d", idx);
            }
        }
        //동작 끝날 때 P줄이 열려있으면 닫아주기
        if (isPrintedP)
            cout << endl;
    }

    return 0;
}