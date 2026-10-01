
#include "Money.h"
Money::Money(int dollars, int cents) : dollars(dollars), cents(cents)
{
    normalize();
}

Money &Money::operator+=(Money other)
{
    dollars += other.dollars;
    cents += other.cents;
    normalize();
    return *this;
}
Money &Money::operator-=(Money other)
{
    dollars -= other.dollars;
    cents -= other.cents;
    normalize();
    return *this;
}

Money Money::operator+(Money other) const
{
    return Money(dollars + other.dollars, cents + other.cents);
}
Money Money::operator-(Money other) const
{
    return Money(dollars - other.dollars, cents - other.cents);
}

void Money::normalize()
{
    dollars += (cents / 100) - (cents % 100 < 0);
    cents = (cents % 100 + 100) % 100;
}

bool Money::operator<=(Money other) const
{
    return (dollars < other.dollars) || (dollars == other.dollars && cents <= other.cents);
}
bool Money::operator<(Money other) const
{
    return (dollars < other.dollars) || (dollars == other.dollars && cents < other.cents);
}
bool Money::operator==(Money other) const
{
    return dollars == other.dollars && cents == other.cents;
}

bool Money::operator!=(Money other) const
{
    return dollars != other.dollars || cents != other.cents;
}
bool Money::operator>=(Money other) const
{
    return (dollars > other.dollars) || (dollars == other.dollars && cents >= other.cents);
}
bool Money::operator>(Money other) const
{
    return (dollars > other.dollars) || (dollars == other.dollars && cents > other.cents);
}

std::string Money::toString() const
{
    return std::format("${}.{:02d}", dollars + (dollars < 0 && cents > 0), (dollars < 0 && cents != 0) ? 100 - cents : cents);
}

// int main()
// {
//     using namespace std;
//     Money m1, m2;
//     cin >> m1;
//     cout << m1 << endl;
//     cin >> m2;
//     cout << m2.toString() << endl;
//     cout << m1 << " + " << m2 << " = " << m1 + m2 << endl;
//     cout << m1 << " - " << m2 << " = " << m1 - m2 << endl;
//     cout << m1 << " == " << m2 << " is ";
//     if (m1 == m2)
//         cout << "true" << endl;
//     else
//         cout << "false." << endl;
//     return 0;
// }