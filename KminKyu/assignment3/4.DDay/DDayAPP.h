#pragma once

#include "DDay.h"

class DDayAPP {
public:
    DDayAPP(const DDay& dday = DDay()) : dday(dday) {}

    void run();

private:
    DDay dday;
    static constexpr int MAX_ANSWER_LENGTH = 9;

    static void printMenu();

    static char toUpper(char alphabet);

    static int changeCharToInt(const char answer[], int startIndex);

    void menu(char command[]);
    
    bool trySetStartDay(int yyyymmdd);


};