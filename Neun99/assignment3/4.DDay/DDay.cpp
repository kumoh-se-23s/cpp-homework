#include <iostream>
#include "DDay.h"
#include "CharUtil.h"
#include <limits>

using namespace std;

void DDay::run() {
    char userInput[INPUT_MAX_LENGTH];
    isRunning = true;

    while (isRunning) {
        printMenu();
        cin.getline(userInput, INPUT_MAX_LENGTH);
        if (cin.fail()) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            printError();
        } else {
            handleInput(userInput);
        }
    }
}

//private-------------------
//메뉴 출력
void DDay::printMenu() const{
    //첫줄
    cout << "<< " << currentDay << " ";
    cout << "[D-day:";
    if (dDay >= 0)
        cout << "+";
    cout << dDay << "] ";
    cout << currentDay + dDay << endl;

    //둘째줄
    cout << ">> Move date{yyyymmdd, Tomorrow(T/t), Yesterday(Y/y)}, Set D-day(+/-int), or Quit(Q/q) : ";
}

//에러 출력
void DDay::printError() const {
    cout << "*** ERROR" << endl;
}

//주 동작
void DDay::handleInput(const char input[]) {
    //한 글자 입력 처리 (tasdf 등 입력 대비)
    int inputSize = CharUtil::getLength(input, INPUT_MAX_LENGTH);
    if (inputSize == 1) {
        switch (input[0]) {
            //tomorrow
            case 'T':
            case 't':
                ++currentDay;
                break;
            //yesterday
            case 'Y':
            case 'y':
                --currentDay;
                break;
            //quit
            case 'Q':
            case 'q':
                isRunning = false;
                cout << "=== END ===" << endl;
                break;
            default:
                printError();
        }
    } else {
        switch (input[0]) {
            case '+':
            case '-':
                setDDay(input, inputSize);
                break;
            default:
                //8자리면서 입력 전부 숫자면 moveDate 동작
                if (inputSize == INPUT_MAX_LENGTH - 1 && CharUtil::isAllDigit(input, 0, INPUT_MAX_LENGTH - 1))
                    moveDate(input);
                else //그 외 에러처리
                    printError();
        }
    }
}

//moveDate 동작
void DDay::moveDate(const char input[]) {
    int newYear = CharUtil::toInt(input, 0, 4);
    int newMonth = CharUtil::toInt(input, 4, 6);
    int newDay = CharUtil::toInt(input, 6, 8);

    //setValue 시도, 유효하지 않은 값이면 에러
    if (!currentDay.setValue(newYear, newMonth, newDay))
        printError();
}

//+ - 연산 동작
void DDay::setDDay(const char input[], int inputSize) {
    if (CharUtil::isAllDigit(input, 1, inputSize)) {
        int value = CharUtil::toInt(input, 1, inputSize);

        if (input[0] == '+')
            dDay = value;
        else
            dDay = -value;
    } else //+- 이후로의 입력이 전부 숫자가 아니면 에러
        printError();
}