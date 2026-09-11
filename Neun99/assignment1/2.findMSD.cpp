#include <iostream>

using namespace std;

int main() {
    int runCntGoal;
    cin >> runCntGoal;

    for(int runCnt = 0; runCnt < runCntGoal; ++runCnt) {
        int userInput;
        cin >> userInput;

        //가장 큰 자릿수 구하기
        int digit = 1;
        while(userInput > 10) {
            userInput /= 10;
            digit++;
        }

        printf("%d @ [%d]\n", userInput, digit);
    }
}