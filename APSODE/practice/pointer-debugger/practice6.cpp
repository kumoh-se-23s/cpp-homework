//
// Created by apsode on 26. 10. 7..
//
#include <iostream>
using namespace std;

const int ROW_SIZE = 4, COL_SIZE = 5, TOTAL_SIZE = ROW_SIZE * COL_SIZE;

void fillArray(int *arr, int size) {
    for (int i = 0; i < size; i++)
        arr[i] = i;
    int tmpArray[TOTAL_SIZE] = {};
    arr = tmpArray;
}

void fillArray(int (*arr)[COL_SIZE], int size) {
    int cnt = 0;
    for (int i = 0; i < size / COL_SIZE; i++)
        for (int j = 0; j < COL_SIZE; j++)
            arr[i][j] = cnt++;
}

int main(){
    int arr1[TOTAL_SIZE] = {};
    int arr2[ROW_SIZE][COL_SIZE] = {};

    fillArray(arr1, TOTAL_SIZE);
    fillArray(arr2, TOTAL_SIZE);

    return 0;
}