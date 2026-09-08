#include <iostream>
#include <string>

using namespace std;

int main() {
    int num;
    double value1 = 78.5, value2 = 3.14150265359;
    cout << value1 << " " << value2 << endl;
    cout.setf(ios::fixed);
    cout.setf(ios::showpoint);
    cout.precision(2);
    cout << value1 << " " << value2 << endl;

    cout.precision(10);
    cout << value1 << " " << value2 << endl;
    printf("%.3f %.6f\n", value1, value2);
    return 0;

}