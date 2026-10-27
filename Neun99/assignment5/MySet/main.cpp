#include "MySet.h"

int main() {
    MySet s1, s2, resultSet;

    cin >> s1 >> s2;
    cout << "s1: " << s1 << endl;
    cout << "s2: " << s2 << endl;

    resultSet = s1 + s2;
    cout << "Union: " << resultSet << endl;

    resultSet = s1 & s2;
    cout << "Intersection: " << resultSet << endl;

    resultSet = s1 - s2;
    cout << "Difference: " << resultSet << "or";
    resultSet = s2 - s1;
    cout << resultSet << endl;
}