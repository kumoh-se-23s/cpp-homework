#include <iostream>
#include "Fraction.h"
using namespace std;

int main()
{
    Fraction f1, f2(2,-5), f3 ;
    f1.set(2,3) ;
    f3 = f1+f2 ;
    cout << f1 << " + " << f2 ;
    cout << " = " << f3 << endl ;
    return 0 ;
}
