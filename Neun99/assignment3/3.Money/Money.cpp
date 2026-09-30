#include <iostream>
#include "Money.h"

using namespace std;

istream& operator>>(istream& in, Money& money) {
    int dollar, cent;
    in >> dollar;
    in >> cent;

    money.setDollar(dollar);
    money.setCent(cent);
}

ostream& operator<<(ostream&, Money& money) {
    
}