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
    void moveDate(const char[]);
    void calculate(const char[], int);
    
    static int toInt(const char[], int, int);
    static int getLength(const char[]);
    static bool isDigit(char);
    static bool isAllDigit(const char [], int, int);
};