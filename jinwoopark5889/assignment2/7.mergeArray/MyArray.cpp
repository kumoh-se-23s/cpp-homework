#include "MyArray.h"

#include <iostream>
using namespace std;

int MyArray1::getArrValue(int index)
{
    return arr[index];
}
int MyArray2::getArrValue(int index)
{
    return arr[index];
}
void MyArray2::setArrValue(int index, int value)
{
    arr[index] = value;
}
bool MyArray1::readAscSorted()
{
    for (int i = 0; i < MAX_SIZE; i++)
        cin >> arr[i];
    for (int i = 0; i < MAX_SIZE; i++)
        if (i != 0 && arr[i] < arr[i - 1])
            return false;
    return true;
}
MyArray2 MyArray1::merge(MyArray1 otherArr)
{
    MyArray2 mergedArr;
    int thisIdx = 0;
    int otherIdx = 0;
    while (thisIdx != MAX_SIZE && otherIdx != MAX_SIZE)
    {
        if (this->arr[thisIdx] < otherArr.getArrValue(otherIdx))
        {
            mergedArr.setArrValue(thisIdx + otherIdx, this->arr[thisIdx]);
            thisIdx++;
        }
        else if (this->arr[thisIdx] > otherArr.getArrValue(otherIdx))
        {
            mergedArr.setArrValue(thisIdx + otherIdx, otherArr.getArrValue(otherIdx));
            otherIdx++;
        }
        else if (this->arr[thisIdx] == otherArr.getArrValue(otherIdx))
        {
            mergedArr.setArrValue(thisIdx + otherIdx, this->arr[thisIdx]);
            thisIdx++;
            mergedArr.setArrValue(thisIdx + otherIdx, otherArr.getArrValue(otherIdx));
            otherIdx++;
        }
    }
    for (; otherIdx < MAX_SIZE; otherIdx++)
        mergedArr.setArrValue(thisIdx + otherIdx, otherArr.getArrValue(otherIdx));

    for (; thisIdx < MAX_SIZE; thisIdx++)
        mergedArr.setArrValue(thisIdx + otherIdx, this->arr[thisIdx]);

    return mergedArr;
}
void MyArray2::print() const
{
    for (int i = 0; i < MAX_SIZE; i++)
        cout << arr[i] << " ";
    cout << endl;
}