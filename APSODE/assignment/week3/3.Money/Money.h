//
// Created by leegu on 26. 10. 7..
//

#ifndef CPP_HOMEWORK_MONEY_H
#define CPP_HOMEWORK_MONEY_H
#include <iostream>

class Money {
    public:
        Money();

        Money(int dollar, int cent);

        int get_dollar() const;

        int get_cent() const;

        void set(int dollar, int cent);

        Money operator +(const Money &money_delta) const;

        Money operator -(const Money &money_delta) const;

        bool operator <=(const Money &other_money) const;

        bool operator >=(const Money &other_money) const;

        bool operator ==(const Money &other_money) const;

        bool operator <(const Money &other_money) const;

        bool operator >(const Money &other_money) const;

        Money &operator =(const Money &money);

    private:
        int dollar = 0;
        int cent = 0;

        Money plus(const Money &money_delta) const;

        Money minus(const Money &money_delta) const;

        int compare_to(const Money &other_money) const;

        void normalize();
};

std::ostream &operator <<(std::ostream &output_stream, const Money &money);

std::istream &operator >>(std::istream &input_stream, Money &money);

#endif //CPP_HOMEWORK_MONEY_H
