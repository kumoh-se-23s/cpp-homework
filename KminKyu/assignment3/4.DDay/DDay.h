#include<iostream>
#include "Day.h"

class DDay {
public:
    DDay(Day sDay = Day(), Day eDay = Day(), int dday = 0);
    void setDDay(int dday);
    void setStartDay(const Day& startDay);
    void setTomarrow();
    void setYesterDay();

private:
    Day startDay = Day();
    Day endDay = Day();
    int dday;


};



