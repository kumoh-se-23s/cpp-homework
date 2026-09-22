#include <iostream>
using namespace std;

bool readArray(int[], int, int);
bool getMinMax(int[], int[], int, int &, int &);
int main()
{
    const int SIZE = 5, ASCENDING = 1, DESCENDING = 2;
    int cnt, arr1[SIZE], arr2[SIZE], min = 0, max = 0;
    cin >> cnt;
    for (int i = 0; i < cnt; ++i)
    {
        if (readArray(arr1, SIZE, ASCENDING) && readArray(arr2, SIZE, DESCENDING))
            if (getMinMax(arr1, arr2, SIZE, min, max))
                cout << "Min : " << min << ", Max : " << max << "\n";
            else
                cout << "ALL SAME : " << min << "\n";
        else
            cout << "[Error] Unsorted input\n";
    }
    return 0;
}
bool readArray(int arr[], int SIZE, int type)
{
    if (type == 1)
    {
        for (int i = 0; i < SIZE; i++)
            cin >> arr[i];
        for (int i = 0; i < SIZE; i++)
            if (i != 0 && arr[i - 1] > arr[i])
                return false;
    }
    else if (type == 2)
    {
        for (int i = 0; i < SIZE; i++)
            cin >> arr[i];
        for (int i = 0; i < SIZE; i++)
            if (i != 0 && arr[i - 1] < arr[i])
                return false;
    }
    return true;
}
bool getMinMax(int arr1[], int arr2[], int SIZE, int &min, int &max)
{
    min = max = arr1[0];
    for (int i = 0; i < SIZE; i++)
    {
        if (i != 0 && arr1[i] > max)
            max = arr1[i];
        
        else if (i != 0 && arr1[i] < min)
            min = arr1[i];
    }
    for (int i = 0; i < SIZE; i++)
    {
        if (i != 0 && arr2[i] > max)
            max = arr2[i];
        
        else if (i != 0 && arr2[i] < min)
            min = arr2[i];
    }
    if (min == max)
        return false;
    else
        return true;
}
