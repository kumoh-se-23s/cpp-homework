#pragma once
#include <iosfwd>

class Money {
    public:
        Money();
        Money(int dollar, int cent);

        int getDollar() const;
        int getCent() const;

        void set(int dollar, int cent);

        Money operator+(const Money& m) const;
        Money operator-(const Money& m) const;

        bool operator!=(const Money& m) const;
        bool operator==(const Money& m) const;
        bool operator>=(const Money& m) const;
        bool operator<=(const Money& m) const;
        bool operator<(const Money& m) const;
        bool operator>(const Money& m) const;

        [[nodiscard]] int abs(int num) const;

    private:
        int dollar;
        int cent;
        void normalize();
};

std::ostream& operator<<(std::ostream& os, const Money& m);
std::istream& operator>>(std::istream& is, Money& m);

