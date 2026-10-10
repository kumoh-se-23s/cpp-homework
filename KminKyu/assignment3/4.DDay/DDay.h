#pragma once

#include<iostream>
#include "Day.h"

class DDay {
public:
    DDay();

    void setDDay(int dday);

    bool setStartDay(int year, int month, int day);

    void setTomorrow();

    void setYesterday();

    const Day& getStartDay() const;

    const Day& getEndDay() const;

    int getDDay() const;

private:
    Day startDay = Day();
    Day endDay = Day();
    int dday;
};

std::ostream &operator<< (std::ostream &out, const DDay &dday);



