#pragma once
#include <iostream>

class Money {
    public:
        Money();
        Money(int dollar, int cent);

        int getDollar() const;
        int getCent() const;

        void set(int dollar, int cent);

        const Money operator+(const Money& m) const;
        const Money operator-(const Money& m) const;

        bool operator!=(const Money& m) ;
        bool operator==(const Money& m) const;
        bool operator>=(const Money& m) const;
        bool operator<=(const Money& m) const;
        bool operator<(const Money& m) const;
        bool operator>(const Money& m) const;

    private:
        int dollar;
        int cent;
        void normalize();
        int abs(int num) const;

};

std::ostream& operator<<(std::ostream& os, const Money& m);
std::istream& operator>>(std::istream& is, Money& m);

