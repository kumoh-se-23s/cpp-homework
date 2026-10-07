#pragma once
#include<iostream>

using namespace std;
class Money {
public:
    Money();
    Money(int d, int c);
    void set(int d, int c);
    Money operator +(const Money& money) const;
    Money operator -(const Money& money) const;
    bool operator <=(const Money& money) const;
    bool operator >=(const Money& money) const;
    bool operator ==(const Money& money) const;
    bool operator <(const Money& money) const;
    bool operator >(const Money& money) const;
    Money& operator =(const Money& money);
    std::string toString() const;
private:
    int dollar;
    int cent;
    Money add(const Money& money) const;
    Money minus(const Money& money) const;
    void normalize();
};

ostream& operator <<(ostream& out, const Money& money);
istream& operator >>(istream& in, Money& money);