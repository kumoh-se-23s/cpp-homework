#include <iostream>
#include "MySet.h"
using namespace std;

int main()
{
    
    MySet s1,s2,resultSet;

    cin >> s1 >> s2;

    resultSet = s1 + s2;
    cout << "Union : " << resultSet << endl;
    
    resultSet = s1  & s2;
    cout << "Intersection : " << resultSet << endl;

    resultSet = s1 - s2;
    cout << "Difference : " << resultSet << " or ";
    
    resultSet = s2 - s1;
    cout << resultSet << endl;

    return 0;
}
//파괴자와 delete