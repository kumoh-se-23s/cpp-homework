#include <iostream>
using namespace std ;

bool readAscSortedArray(int [], int);
void mergeArray(int [], int [], int [], int);
void printArray(int [], int);

int main() {
    const int SIZE = 5;
    int cnt, arr1[SIZE], arr2[SIZE], resultArr[SIZE * 2];
    cin >> cnt;

    for (int i = 0; i < cnt; ++i) {
        if (readAscSortedArray(arr1, SIZE) && readAscSortedArray(arr2, SIZE)) {
            mergeArray(arr1, arr2, resultArr, SIZE);
            printArray(resultArr, SIZE * 2);
        } else cout << "[Error] Unsorted input\n";
    }

    return 0;
}

bool readAscSortedArray(int arr[], int size) {
    // bool isRun = false;
    // do {
    for (int i = 0; i < size; i++) {
        cin >> arr[i];
    }

    for (int i = 0; i < size - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            // cout << "[ERROR] Unsorted input" << endl;
            return false;
            // isRun = true;
            // break;
        }
        // isRun = false;
    }
    // } while (isRun);
    return true;
}

void mergeArray(int arr1[], int arr2[], int arr3[], int size) {
    int idx = 0, i = 0, j = 0;

    while (i < size && j < size) {
        if (arr1[i] < arr2[j]) {
            arr3[idx++] = arr1[i++];
        } else {
            arr3[idx++] = arr2[j++];
        }
    }
    while (i < size) {
        arr3[idx++] = arr1[i++];
    }
    while (j < size) {
        arr3[idx++] = arr2[j++];
    }
}

void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
}
