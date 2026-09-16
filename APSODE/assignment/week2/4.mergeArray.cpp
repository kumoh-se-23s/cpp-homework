//
// Created by leegu on 26. 9. 16..
//
#include <iostream>
using namespace std ;

bool is_asc_sorted(int array[], int max_size) {
    if (max_size == 1) {
        return true;
    }

    for (int index = 1; index < max_size; ++index) {
        int before_element = array[index - 1];
        int current_element = array[index];

        if (before_element > current_element) {
            return false;
        }
    }

    return true;
} // 기본 built-in 함수들이 snake-case를 사용하고 있기에 snake-case로 작성하였습니다.

void print_integer_array(int integer_array[], int max_length = 0) {
    for (int index = 0; index < max_length; ++index) {
        std::cout << integer_array[index] << " ";
    }
    std::cout << std::endl;
} // 기본 built-in 함수들이 snake-case를 사용하고 있기에 snake-case로 작성하였습니다.


bool readAscSortedArray(int array[], int array_size) {
    for (int index = 0; index < array_size; ++index) {
        std::cin >> array[index];
    }

    return is_asc_sorted(array, array_size);
}

void mergeArray(int array1[], int array2[], int result_array[], int array_size) {
    int arr1_index = 0;
    int arr2_index = 0;
    int result_array_index = 0;
    for (
        ;
        arr1_index < array_size && arr2_index < array_size;
        ++result_array_index
    ) {
        result_array[result_array_index] = array1[arr1_index] > array2[arr2_index]
                                        ? array1[arr1_index++]
                                        : array2[arr2_index++];
    }

    for (; arr1_index < array_size; ++arr1_index) result_array[result_array_index++] = array1[arr1_index++];
    for (; arr2_index < array_size; ++arr2_index) result_array[result_array_index++] = array2[arr2_index++];
}
void printArray(int [], int) ;


int main() {
    const int SIZE = 5 ;
    int cnt, arr1[SIZE], arr2[SIZE], resultArr[SIZE*2];
    cin >> cnt ;
    for (int i = 0 ; i < cnt ; ++i) {
        if (readAscSortedArray(arr1, SIZE) && readAscSortedArray(arr2, SIZE)) {
            mergeArray(arr1, arr2, resultArr, SIZE) ;
            printArray(resultArr, SIZE*2) ;
        }
    }
    return 0 ;
}