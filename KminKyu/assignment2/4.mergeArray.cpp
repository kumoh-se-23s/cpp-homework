#include <iostream>

using namespace std;

bool readAscSortedArray(int [], int) ;
void mergeArray(int [], int [], int[], int) ;
void printArray(int [], int) ;

int main() {
    const int SIZE = 5 ;

    int cnt, arr1[SIZE], arr2[SIZE], resultArr[SIZE*2];

    cin >> cnt ;

    for (int i = 0 ; i < cnt ; ++i) {
        if ( readAscSortedArray(arr1, SIZE) && readAscSortedArray(arr2, SIZE) ) {
            mergeArray(arr1, arr2, resultArr, SIZE) ;
            printArray(resultArr, SIZE*2) ;
        }
        else cout << "[Error] Unsorted input\n";
    }
    return 0 ;
}

bool readAscSortedArray(int arr[], int size) {
    for (int arrSize = 0; arrSize < size; ++arrSize) {
        cin >> arr[arrSize];
    }
    for (int arrSize = 0; arrSize < size - 1; ++arrSize) {
        if (arr[arrSize] > arr[arrSize + 1]) {
            return false;
        }
    }
    return true;
}

void mergeArray(int firstArray[], int secondArray[], int mergedArray[], int size) {
    int firstArrayIndex = 0, secondArrayIndex = 0, arrayIndex = 0;
    for (arrayIndex = 0; arrayIndex < size * 2 && firstArrayIndex < size && secondArrayIndex < size; ++arrayIndex) {
        if (firstArray[firstArrayIndex] <= secondArray[secondArrayIndex]) {
            mergedArray[arrayIndex] = firstArray[firstArrayIndex++];
        } else {
            mergedArray[arrayIndex] = secondArray[secondArrayIndex++];
        }
    }
    if (firstArrayIndex < size) {
        while (firstArrayIndex < size) {
            mergedArray[arrayIndex++] = firstArray[firstArrayIndex++];
        }
    } else {
        while (secondArrayIndex < size) {
            mergedArray[arrayIndex++] = secondArray[secondArrayIndex++];
        }
    }
}

void printArray(int array[], int size) {
    for (int index = 0; index < size; ++index) {
        if (index != 0) {
            cout << " ";
        }
        cout << array[index];
    }
    cout << endl;
}