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

        DDay(char date_text[]);

        Day get_day() const;

        int get_day_delta() const;

        void tomorrow();

        void yesterday();

        void set_dday(int day_delta);

        Day calc_dday() const;

    private:
        Day day;
        int day_delta;
};

std::ostream& operator<<(std::ostream &output_stream, const DDay &dday);

#endif //CPP_HOMEWORK_DDAY_H
