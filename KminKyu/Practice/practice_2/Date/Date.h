#pragma once
#include<iostream>
using namespace std;

class Date {
public:
    Date(){};
    Date(int y, int m = 1, int d = 1);

    void setDay(int d);
    void setMonth(int m);
    void setYear(int y);
    void read();
    string getString() const;
private:
    void setDayOfWeek();
    int year = 1, month = 1, day = 1, dayOfWeek = 1;
    static const int MAX_MONTH = 12;
};