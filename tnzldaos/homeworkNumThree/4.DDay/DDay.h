#pragma once
#include "Day.h"

class DDay {
    public:
    private:
        int dday;

        Day currentDay = Day();

    public:
        DDay(int inputDDay);
        
        int getDDay() const;
        void setDDay(int inputDDay);
        
        Day getDate() const;
        bool setDate(int inputYear, int inputMonth, int inputDay);

        Day plusDDay() const;
        Day minusDDay() const;

        void moveTomorrow();
        void moveYesterday();


};