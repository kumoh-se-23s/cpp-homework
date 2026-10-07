//
// Created by leegu on 26. 10. 7..
//
#include <iostream>
#include <Fraction.h>


int main() {
    Fraction f1;
    Fraction f2 = Fraction(2, -5);
    Fraction f3;
    f1.set(2,3) ;
    f3 = f1+f2 ;
    std::cout << f1 << " + " << f2 ;
    std::cout << " = " << f3 << std::endl ;
    return 0;
}

