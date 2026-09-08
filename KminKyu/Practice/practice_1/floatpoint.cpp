#include<iostream>
#include<iomanip>
#include<string>

using namespace std;

int main() {
    double x = 1237;
    double y = 12376745.5623;
    cout << x << endl;
    cout << showpoint << x << endl;
    cout << scientific << setprecision(20) << y << endl;
    
    cout << fixed << setprecision(2) << showpos << setfill('*') << y << endl;
    cout << setw(15) << left << y << endl;
    cout << setw(15) << internal << y << endl;
    cout << setw(15) << right << y << endl;


    return 0;
}