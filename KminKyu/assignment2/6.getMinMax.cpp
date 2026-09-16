#include <iostream>
using namespace std ;
bool readArray(int [], int, int) ; bool getMinMax(int [], int [], int, int&, int&) ;
int main() {
    const int SIZE = 5, ASCENDING = 1, DESCENDING = 2 ;
    int cnt, arr1[SIZE], arr2[SIZE], min=0, max=0 ;
    cin >> cnt ;
    for (int i = 0 ; i < cnt ; ++i) {
        if ( readArray(arr1, SIZE, ASCENDING) && readArray(arr2, SIZE, DESCENDING) )
            if (getMinMax(arr1, arr2, SIZE, min, max))
                cout << "Min : " << min << ", Max : " << max << "\n" ;
            else
                cout << "두 배열의 모든 값이 동일함" << min << "\n" ;
        else cout << "[Error] Unsorted input\n";
    }
    return 0 ;
}

bool readArray(int arr[], int size, int sortType) {

    for (int arrSize = 0; arrSize < size; ++arrSize) {
        cin >> arr[arrSize];
    }
    for (int arrSize = 0; arrSize < size - 1; ++arrSize) {
        if (sortType == 1) {
            if (arr[arrSize] > arr[arrSize + 1]) {
                return false;
            }
        } else if (sortType == 2) {
            if (arr[arrSize] < arr[arrSize + 1]) {
                return false;
            }
        }
    }
    return true;
}

bool getMinMax(int firstArray[], int secondArray[], int size, int& min, int& max) {
    if (firstArray[0] < secondArray[size - 1]) {
        min = firstArray[0];
    } else {
        min = secondArray[size - 1];
    }

    if (firstArray[size - 1] > secondArray[0]) {
        max = firstArray[size - 1];
    } else {
        max = secondArray[0];
    }

    if (min == max) {
        return false;
    } else {
        return true;
    }
}

