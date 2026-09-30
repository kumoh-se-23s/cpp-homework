#include<iostream>
#include"Matrix.h"

using namespace std;

int main() {
    Matrix m1, m2 ;
    cin >> m1 ; cout << m1 ;
    cin >> m2 ; cout << m2 ;
    cout << "Transpose " << endl ;
    cout << !m1 ;
    cout << "Add " << endl ;
    cout << m1 + m2 ;
    cout << "Multiply " << endl ;
    cout<< m1 * m2 ;
    return 0 ;
}