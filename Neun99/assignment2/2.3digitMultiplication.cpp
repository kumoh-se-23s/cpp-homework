#include <iostream>

using namespace std;

int main() {
    int getNthDigit(int value, int digitPlace);
    int getTenTimesN(int n);

    int userInput1;
    cin >> userInput1;
    int userInput2;
    cin >> userInput2;

    const int ARR_MAX_SIZE = 3;
    int midResultArr[ARR_MAX_SIZE];
    int result = 0;

    //중간결과 채우면서 출력
    for(int idx = 0; idx < ARR_MAX_SIZE; idx++) {
        midResultArr[idx] = userInput1 * getNthDigit(userInput2, idx);
        cout << midResultArr[idx] << endl;
    }
    //최종결과 계산
    for(int idx = 0; idx < ARR_MAX_SIZE; idx++) {
        result += midResultArr[idx] * getTenTimesN(idx);
    }
    //최종결과 출력
    cout << result;
    return 0;
}

//n번째 자리의 숫자 반환(자릿수는 맨 우측부터 0으로 시작)
int getNthDigit(int value, int digitPlace) {
    int result = value;
    for(int idx = 0; idx < digitPlace; idx++) {
        result /= 10;
    }
    return result % 10;
}

//10의 n승 반환
int getTenTimesN(int n) {
    int result = 1;
    for(int idx = 0; idx < n; idx++) {
        result *= 10;
    }
    return result;
}