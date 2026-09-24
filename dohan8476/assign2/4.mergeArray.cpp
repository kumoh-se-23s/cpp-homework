#include <iostream>
using namespace std ;

bool readAscSortedArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        cin >> arr[i];
    }

    for (int i = 0; i < size - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            return false;
        }
    }
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
