#pragma once
#include "Day.h"

class DDayApp {
public:
    void run();
private:
    Day currentDay;
    Day targetDay;
    int dDay = 0;

    void printMenu() const;
    void printError() const;

    void moveDate(const char userInput[]);
    void calculate(const char userInput[], int inputSize);

    //util
    static int toInt(const char arr[], int startIdx, int endIdx);
    static int getLength(const char arr[]);
    static bool isDigit(char text);
    static bool isAllDigit(const char arr[], int startIdx, int endIdx);
};