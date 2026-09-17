#include <iostream>

using namespace std;

int main() {
    int runCntGoal;
    cin >> runCntGoal;

    for(int runCnt = 0; runCnt < runCntGoal; runCnt++) {
        int userInput;
        cin >> userInput;

        const int ARR_MAX_SIZE = 32;
        short bin[ARR_MAX_SIZE];
        int usingArrSize = 0;

        //이진법 계산
        for(; userInput > 0 && usingArrSize < ARR_MAX_SIZE; userInput /= 2, usingArrSize++) {
            bin[usingArrSize] = userInput % 2;
        }

        //출력
        bool isPrinting = false;
        for(int idx = usingArrSize; idx >= 0; idx--) {
            if(!isPrinting && bin[idx] == 1) 
                isPrinting = true;
            if(isPrinting)
                cout << bin[idx];
            if(idx % 4 == 0)
                cout << " ";
        }
        cout << endl;
    }
    return 0;
}