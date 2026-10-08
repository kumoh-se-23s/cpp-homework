#ifndef DDAY_H
#define DDAY_H

#include "DDay.h"

class DDayAPP {
public:
    DDayAPP(DDay dday = DDay()) {}

    void run();

private:
    DDay dday;
    static const int MAX_ANSWER_LENGTH = 9;
    static void printMenu();

    static char toUpper(char alphabet);

    void menu(char command[]) const;

    static int changeCharToInt(const char answer[], int startIndex);
};

#endif