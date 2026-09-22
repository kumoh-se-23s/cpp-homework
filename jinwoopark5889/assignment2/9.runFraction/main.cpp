#include <iostream>

#include "Fraction.h"
int main()
{
    Fraction f1 = Fraction(12, 3);
    Fraction f2 = Fraction(12, 3);
    Fraction f3 = f1.add(f2);
    f3.print();
    return 0;
}
