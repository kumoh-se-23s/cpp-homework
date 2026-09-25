#include <iostream>

using namespace std;

//입력받기
bool readArray(int arr[], int size, int type) {
    for(int idx = 0; idx < size; idx++) {
        cin >> arr[idx];
    }

    //검증
    if(type == 1) { //오름차순
        for(int idx = 0; idx < size - 1; idx++) {
            if(arr[idx] > arr[idx+1])
                return false;
        }
    } else { //내림차순
        for(int idx = 0; idx < size - 1; idx++) {
            if(arr[idx] < arr[idx + 1])
                return false;
        }
    }
    return true;
}

//arr1은 오름, arr2는 내림차순으로 고정
bool getMinMax(int arr1[], int arr2[], int size, int& min, int& max) {
    //최솟값 찾기
    if (arr1[0] < arr2[size - 1])
        min = arr1[0];
    else
        min = arr2[size - 1];

    //최댓값 찾기
    if (arr1[size - 1] < arr2[0])
        max = arr2[0];
    else
        max = arr1[size - 1];

    //최대최소 동일하면 false 리턴
    if (min == max)
        return false;
    return true;
}