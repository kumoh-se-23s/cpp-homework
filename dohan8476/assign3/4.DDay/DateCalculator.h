#pragma once

class DateCalculator {
    public:
        bool isLeapYear(int year);
        bool isValidDate(int year, int month, int day);
        int dateToSerial(int year, int month, int day);
        int serialToDate(int serial);
};
