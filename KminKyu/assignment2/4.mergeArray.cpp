#include <iostream>
using namespace std ;
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
        else cout << "[Error] Unsorted input\n" ;
    }
    return 0 ;
}

bool readAscSortedArray(int arr[], int size) {
    bool flag = true;
    for (int arrSize = 0; arrSize < size; ++arrSize) {
        cin >> arr[arrSize];
        if (arrSize != 0 && arr[arrSize - 1] > arr[arrSize]) {
            flag = false;
        }
    }
    return flag;
}

void mergeArray(const int firstArray[], const int secondArray[], int mergedArray[], int size) {
    int firstArrayIndex = 0, secondArrayIndex = 0, arrayIndex = 0;

    for (arrayIndex = 0; arrayIndex < size * 2 && firstArrayIndex < size && secondArrayIndex < size; ++arrayIndex) {
        if (firstArray[firstArrayIndex] <= secondArray[secondArrayIndex]) {
            mergedArray[arrayIndex] = firstArray[firstArrayIndex++];
        } else {
            mergedArray[arrayIndex] = secondArray[secondArrayIndex++];
        }
    }
    while (firstArrayIndex < size) {
        mergedArray[arrayIndex++] = firstArray[firstArrayIndex++];
    }
    while (secondArrayIndex < size) {
        mergedArray[arrayIndex++] = secondArray[secondArrayIndex++];
    }
}

void printArray(const int array[], int size) {
    for (int index = 0; index < size; ++index) {
        if (index != 0) {
            cout << " ";
        }
        cout << array[index];
    }
    cout << endl;
}