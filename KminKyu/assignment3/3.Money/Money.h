#pragma once
#include<iostream>

using namespace std;
class Money {
public:
    Money();
    Money(int d, int c);
    int getDollar() const;
    int getCent() const;
    void set(int d, int c);
    Money operator +(const Money& money) const;
    Money operator -(const Money& money) const;
    bool operator <=(const Money& money) const;
    bool operator >=(const Money& money) const;
    bool operator ==(const Money& money) const;
    bool operator <(const Money& money) const;
    bool operator >(const Money& money) const;
    Money& operator =(const Money& money);
private:
    int dollar = 0;
    int cent = 0;
    Money add(const Money& money) const;
    Money minus(const Money& money) const;
    void normalize();
};

ostream& operator <<(ostream& out, const Money& money);
istream& operator >>(istream& in, Money& money);