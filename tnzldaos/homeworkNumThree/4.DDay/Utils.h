#pragma once
#include "Day.h"
#include "DDay.h"

class Utils {
    public:
        Utils();

        bool isDDay(char inputCharArr[], int length);
        bool isValidInput(char inputCharArr[]);
        int charArrParser(char inputCharArr[], int arrLength);
};  

ostream& operator<<(ostream& outputStream, const Day& day);
ostream& operator<<(ostream& outputStream, const DDay& dday);