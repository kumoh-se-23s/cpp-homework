//
// Created by leegu on 26. 10. 7..
//

#ifndef CPP_HOMEWORK_DDAY_H
#define CPP_HOMEWORK_DDAY_H

#include "Day.h"


class DDay {
    public:
        DDay();

        DDay(Day manual_day);

        void tomorrow();

        void yesterday();

        void set_dday(int day_delta);

        Day calc_dday();

    private:
        Day day;
        int day_delta;


};
#endif //CPP_HOMEWORK_DDAY_H
