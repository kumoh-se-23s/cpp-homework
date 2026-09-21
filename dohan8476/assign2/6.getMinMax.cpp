#include <iostream>
using namespace std ;

bool readArray(int [], int, int);

bool getMinMax(int [], int [], int, int&, int&);

int main() {
    const int SIZE = 5, ASCENDING = 1, DESCENDING = 2;
    int cnt, arr1[SIZE], arr2[SIZE], min = 0, max = 0;
    cin >> cnt;
    for (int i = 0; i < cnt; ++i) {
        if (readArray(arr1, SIZE, ASCENDING) && readArray(arr2, SIZE, DESCENDING))
            if (getMinMax(arr1, arr2, SIZE, min, max))
                cout << "Min : " << min << " , Max : " << max << " \n" ;
            else
                cout << "ALL SAME" << min << "\n" ;
            else cout << " [Error]Unsorted input\n" ;
    }
}

bool getMinMax(int ascend[], int descend[], int size, int& min, int& max) {
    if (ascend[0] > descend[0]) {
        min = descend[size-1];
        max = ascend[size-1];
    }
    else {
        min = ascend[0];
        max = descend[0];

        for (int i = 1; i < size; i++) {
            if (min > descend[i]) {
                min = descend[i];
            }
            if (max < ascend[i]) {
                max = ascend[i];
            }
        }
    }
    if (min == max) {
        return false;
    }
    return true;
}

bool readArray(int arr[], int size, int ascend) {
    for (int i = 0; i < size; i++) {
        cin >> arr[i];
    }

    if (ascend == 1) {
        for (int i = 0; i < size - 1; i++) {
            if (arr[i] > arr[i + 1]) {
                return false;
            }
        }
        return true;
    }
    for (int i = 0; i < size - 1; i++) {
        if (arr[i] < arr[i + 1]) {
            return false;
        }
    }
    return true;
}
