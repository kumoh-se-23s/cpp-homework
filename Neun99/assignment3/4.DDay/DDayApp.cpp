#include <iostream>
#include "DDayApp.h"

using namespace std;

void DDayApp::run() {
    bool isRunning = true;
    constexpr int INPUT_MAX_LENGTH = 9;
    char userInput[INPUT_MAX_LENGTH];

    while (isRunning) {
        printMenu();
        cin.getline(userInput, INPUT_MAX_LENGTH);

        //동작부
        //한 글자 입력 처리 (tasdf 등 입력 대비)
        int inputSize = getLength(userInput);
        if (inputSize == 1) {
            switch (userInput[0]) {
                //tomorrow
                case 'T':
                case 't':
                    ++currentDay;
                    ++targetDay;
                    break;
                //yesterday
                case 'Y':
                case 'y':
                    --currentDay;
                    --targetDay;
                    break;
                //quit
                case 'Q':
                case 'q':
                    isRunning = false;
                    break;
                default:
                    printError();
            }
        } else {
            switch (userInput[0]) {
                case '+':
                case '-':
                    calculate(userInput, inputSize);
                    break;
                default:
                    //입력 전부 숫자면 moveDate 동작
                    if (isAllDigit(userInput, 0, INPUT_MAX_LENGTH - 1))
                        moveDate(userInput);
                    else //그 외 에러처리
                        printError();
            }
        }
    }
}

//private-------------------
//메뉴 출력
void DDayApp::printMenu() const{
    //첫줄
    cout << "<< " << currentDay << " ";
    cout << "[D-day";
    if (dDay >= 0)
        cout << "+";
    cout << dDay << "] ";
    cout << targetDay << endl;

    //둘째줄
    cout << ">> Move date{yyyymmdd, Tomorrow(T/t), Yesterday(Y/y)}, Set D-day(+/-int), or Quit(Q/q):  ";
}

//에러 출력
void DDayApp::printError() const {
    cout << "*** ERROR" << endl;
}

//moveDate 동작
void DDayApp::moveDate(const char userInput[]) {
    int newYear = toInt(userInput, 0, 4);
    int newMonth = toInt(userInput, 4, 6);
    int newDay = toInt(userInput, 6, 8);

    //setValue 시도, 유효하지 않은 값이면 에러
    if (currentDay.setValue(newYear, newMonth, newDay))
        targetDay = currentDay + dDay;
    else
        printError();
}

//+ - 연산 동작
void DDayApp::calculate(const char userInput[], int inputSize) {
    //+- 이후로의 입력이 전부 숫자가 아니면 에러
    if (isAllDigit(userInput, 1, inputSize)) {
        int value = toInt(userInput, 1, inputSize);

        if (userInput[0] == '+') {
            targetDay = currentDay + value;
            dDay = value;
        }
        else {
            targetDay = currentDay - value;
            dDay = -value;
        }
    } else
        printError();
}

//배열의 지정된 범위 int로 변환
int DDayApp::toInt(const char arr[], int startIdx, int endIdx) {
    int result = 0;
    for (int idx = startIdx; idx < endIdx; idx++) {
        if (!isDigit(arr[idx])) //
            break;
        result = result * 10 + (arr[idx] - '0');
    }
    return result;
}

//배열에서 실제로 사용중인 길이 반환
int DDayApp::getLength(const char arr[]){
    int idx = 0;
    while (arr[idx] != '\0') {
        idx++;
    }
    return idx;
}

//글자 하나 숫자인지 반환
bool DDayApp::isDigit(const char input){
    int result = input - '0';
    return (result >= 0 && result <= 9);
}

//배열에서 지정한 범위가 다 숫자인지 반환
bool DDayApp::isAllDigit(const char arr[], int startIdx, int endIdx){
    for (int idx = startIdx; idx < endIdx; idx++) {
        if (!isDigit(arr[idx]))
            return false;
    }
    return true;
}
