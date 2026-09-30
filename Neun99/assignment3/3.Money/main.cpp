#include <iostream>
#include "Money.h"

using namespace std;

int main() {
    Money m1, m2;
    cin >> m1 ;
    cout << m1 << endl ;
    cin >> m2 ;
    cout << m2.toString() << endl ;
    cout << m1 << " + " << m2 << " = " << m1 + m2 << endl ;
    cout << m1 << " - " << m2 << " = " << m1 - m2 << endl ;
    cout << m1 << " == " << m2 << " is ";
    if (m1 == m2) cout << "true" << endl;
    else cout << "false." << endl;
}