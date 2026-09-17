#include <iostream>
#include "Fraction.h"
int main()
{
    Fraction f1, f2(5, -4), f3 ; // f2 = 2/5

    f1.set(2,6) ;  // f1 = 2/3
    f3 = f1.add(f2) ; // f1 = 2/3
    f3.print() ;
    return 0 ;
}