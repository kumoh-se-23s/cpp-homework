#include <iostream>
#include <cmath>
using namespace std;
bool readArray(int arr[], int size, int mode){
    constexpr int ASCENDING = 1, DESCENDING = 2; 
    int previousInput = mode == ASCENDING ? 0 : 0x7fffffff;
    for(int i = 0; i < size; ++i){
        cin >> arr[i];
        if((mode == ASCENDING && arr[i] < previousInput) || (mode == DESCENDING && arr[i] > previousInput)) {
            return false;
        }
        previousInput = arr[i];
    }
    return true;
}


bool getMinMax(int arr1[], int arr2[], int size, int & min, int & max) {
    min = std::min(arr1[0], arr2[size - 1]);
    max = std::max(arr1[size - 1], arr2[0]);
    return min != max;
}