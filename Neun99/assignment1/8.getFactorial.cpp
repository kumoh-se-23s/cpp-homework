#include <iostream>

using namespace std;

int main() {
    int getFactorialByIteration(int val);
    int getFactorialByRecursive(int val);

    int runCntGoal;
    cin >> runCntGoal;

    for(int runCnt = 0; runCnt < runCntGoal; runCnt++) {
        int userInput;
        cin >> userInput;

        printf("iteration = %d\n", getFactorialByIteration(userInput));
        printf("recursive = %d\n", getFactorialByRecursive(userInput));
    }
    return 0;
}

int getFactorialByIteration(int val) {
    int result = 1;
    for(int idx = 2; idx <= val; idx++) {
        result *= idx;
    }
    return result;
}

int getFactorialByRecursive(int val) {
    if(val > 1)
        return val * getFactorialByRecursive(val - 1);
    return val;
}
