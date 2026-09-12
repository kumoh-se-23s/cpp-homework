#include <iostream>

using namespace std;

int getFactorialByIteration(int val);
int getFactorialByRecursive(int val);

int main() {
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
    return 1;
}

/*
13! 이상에서 값이 이상하게 나오는 이유와 해결법에 대해

값이 이상하게 나오는 이유는 12! == 479001600, 13! == 6227020800(대략 62억)으로,
13!에서 기본 int의 최대 범위인 2147483647(대략 21억)을 넘어서면서 오버플로우가 발생하기 때문입니다.
arraylist등의 list를 활용해 자릿수와 그 자릿수에 해당하는 숫자의 쌍으로 긴 수를 저장하고 표현하는 식으로 곱셈 기능이 포함된 클래스를 구현해 해결하는 방법을 생각해보았습니다.

사용자가 입력한 수를 arraylist에 자릿수별로 저장합니다. idx는 자릿수를 의미하며, 각 원소에는 해당 자릿수의 숫자가 들어가게 됩니다.
위를 통해 사용자가 int 최댓값에 가까운 숫자를 입력했을 때 2만 곱해도 오버플로우가 발생되는 상황을 막습니다.
같은 이유로 이후 곱해질 수 역시 같은 방식으로 저장해, 자릿수별로 곱셈을 수행해 결과 배열에 더해 저장합니다(10 이상의 수는 맞는 자리로 올려 처리시킵니다).
*/