#include <iostream>
using namespace std;

bool readAscSortedArray(int arr[], int size)
{
    for (int i = 0; i < size; i++)
        cin >> arr[i];

    for (int i = 0; i < size; i++)
        if (i != 0 && arr[i] < arr[i - 1])
            return false;

    return true;
}
void mergeArray(int arr1[], int arr2[], int resultArr[], int size)
{
    int arr1Idx = 0;
    int arr2Idx = 0;

    while (arr1Idx != size && arr2Idx != size)
    {
        if (arr1[arr1Idx] < arr2[arr2Idx])
        {
            resultArr[arr1Idx + arr2Idx] = arr1[arr1Idx];
            arr1Idx++;
        }
        else if (arr1[arr1Idx > arr2[arr2Idx]])
        {
            resultArr[arr1Idx + arr2Idx] = arr2[arr2Idx];
            arr2Idx++;
        }
        else if (arr1[arr1Idx] == arr2[arr2Idx])
        {
            resultArr[arr1Idx + arr2Idx] = arr1[arr1Idx];
            arr1Idx++;
            resultArr[arr1Idx + arr2Idx] = arr2[arr2Idx];
            arr2Idx++;
        }
    }
    for (; arr2Idx < size; arr2Idx++)
        resultArr[arr1Idx + arr2Idx] = arr2[arr2Idx];

    for (; arr1Idx < size; arr1Idx++)
        resultArr[arr1Idx + arr2Idx] = arr1[arr1Idx];
}
void printArray(int resultArr[], int size)
{
    for (int i = 0; i < size; i++)
        cout << resultArr[i] << " ";

    cout << endl;
}