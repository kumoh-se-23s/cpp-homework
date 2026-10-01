#pragma once
#include "Day.h"

class DDay {
public:
    void run();
private:
    Day currentDay;
    Day targetDay;
    int dDay;

    bool isRunning = true;

    void printMenu();

    void moveDate(char[]);
    int toInt(char[]);
    bool isValidDate(int, int, int);
};