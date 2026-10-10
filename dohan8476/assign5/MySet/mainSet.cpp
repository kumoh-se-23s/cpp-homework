#include <iostream>
#include "MySet.h"
using namespace std;

int main() {
    MySet s1, s2, resultSet;
    cin >> s1 >> s2; // 음수가 들어올 때까지 입력
    resultSet = s1 + s2; // 합집합
    cout << "Union : " << resultSet << endl;
    resultSet = s1 & s2; // 교집합
    cout << "Intersection : " << resultSet << endl;
    resultSet = s1 - s2; // 차집합
    cout << "Difference : " << resultSet << " or "  ;
    resultSet = s2 - s1;
    cout << resultSet << endl;
}