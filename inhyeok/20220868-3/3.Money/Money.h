
#pragma once
#include <iostream>
#include <format>

class Money{
    int dollars;
    int cents;

public:

    explicit Money(int dollars = 0, int cents = 0);



    Money &operator+=(Money other);
    Money &operator-=(Money other);

    Money operator+(Money other) const;
    Money operator-(Money other) const;


    void normalize();

    bool operator<=(Money other) const;
    bool operator<(Money other) const;
    bool operator==(Money other) const;
    bool operator!=(Money other) const;
    bool operator>=(Money other) const;
    bool operator>(Money other) const;
    std::string toString() const;
};

inline std::ostream & operator<<(std::ostream &out, const Money &money){
    return out << money.toString();
}
inline std::istream & operator>>(std::istream &in, Money &money){
    int dollars; 
    int cents;
    in >> dollars >> cents;
    money = Money(dollars, cents);
    return in;
}