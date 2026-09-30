#pragma once
#include <iostream>
#include <string>

using namespace std;

class Money {
public:
    Money();
    Money(int, int);

    int getDollar() const;
    int getCent() const;
    void setDollar(int);
    void setCent(int);

    const Money operator+(const Money&) const;
    const Money operator-() const;
    const Money operator-(const Money&) const;
    bool operator==(const Money&) const;
    bool operator!=(const Money&) const;
    bool operator>=(const Money&) const;
    bool operator<=(const Money&) const;
    bool operator>(const Money&) const;
    bool operator<(const Money&) const;
    string toString() const;
private:
    int dollar;
    int cent;
    void normalize(int& dollar, int& cent);
};

ostream& operator<<(ostream&, const Money&);
istream& operator>>(istream&, Money&);