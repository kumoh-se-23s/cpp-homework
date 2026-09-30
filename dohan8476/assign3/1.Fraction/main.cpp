#include <iostream>
#include "Fraction.h"
using namespace std;

int main()
{
    Fraction f1, f2(1,-5000000), f3 ;
    f1.set(1000,0) ;
    f3 = f1+f2 ;
    cout << f1 << " + " << f2 ;
    cout << " = " << f3 << endl ;
    return 0 ;
}
