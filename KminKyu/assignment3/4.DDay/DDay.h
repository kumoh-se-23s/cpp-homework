#include<iostream>
#include "Day.h"

class DDay {
public:
    DDay(Day sDay = Day(), Day eDay = Day(), int dday = 0);
    bool menu(const char command[]);
    void run();
    void PrintMenu();
private:
    Day startDay = Day();
    Day endDay = Day();
    int dday;

    void setDDay(int dday);
    void setStartDay(const Day& startDay);
    void setTomarrow();
    void setYesterDay();
};



