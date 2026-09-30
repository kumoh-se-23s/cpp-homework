#include <iostream>

using namespace std;

#pragma once
class Money {
public:
    Money operator+(Money&);
    Money operator-();
    Money operator-(Money&);
    bool operator==(Money&);
    bool operator!=(Money&);
    bool operator>=(Money&);
    bool operator<=(Money&);
    bool operator>(Money&);
    bool operator<(Money&);
    string toString();

    int getDollar();
    int getCent();
    void setDollar(int);
    void setCent(int);
private:
    int dollar;
    int cent;
};

ostream& operator<<(ostream&, Money&);
istream& operator>>(istream&, Money&);