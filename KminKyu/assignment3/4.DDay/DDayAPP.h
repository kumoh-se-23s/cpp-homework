#ifndef DDAY_H
#define DDAY_H

#include "DDay.h"

class DDayAPP {
public:
    DDayAPP(DDay dday = DDay()) {}

    void run();

private:
    DDay dday;
    static constexpr int MAX_ANSWER_LENGTH = 9;
    static void printMenu();

    static char toUpper(char alphabet);

    bool menu(char command[]);

    static int changeCharToInt(const char answer[], int startIndex);
};

#endif