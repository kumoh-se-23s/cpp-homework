#include <iostream>

using namespace std;

int main() {
    int runCntGoal;
    cin >> runCntGoal;

    for(int runCnt = 0; runCnt < runCntGoal; runCnt++) {
        int userInput;
        cin >> userInput;

        const int ARR_MAX_SIZE = 32;
        int bin[ARR_MAX_SIZE];
        int usingArrSize = 0;

        //이진법 계산
        for(; userInput > 0 && usingArrSize < ARR_MAX_SIZE; userInput /= 2, usingArrSize++) {
            bin[usingArrSize] = userInput % 2;
        }

        //출력
        for(int idx = usingArrSize - 1; idx >= 0; idx--) {
            cout << bin[idx];
            if(idx % 4 == 0)
                cout << " ";
        }
        cout << endl;
    }
    return 0;
}