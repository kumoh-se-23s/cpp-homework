#pragma once
#include "Day.h"

class DDay {
public:
    void run();
private:
    Day currentDay;
    int dDay = 0;
    bool isRunning;
    static constexpr int INPUT_MAX_LENGTH = 9;

    void handleInput(const char input[]);
    void printMenu() const;
    void printError() const;
    void moveDate(const char input[]);
    void setDDay(const char input[], int inputSize);
};