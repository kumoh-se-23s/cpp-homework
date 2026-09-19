//
// Created by leegu on 26. 9. 16..
//
#include <iostream>
using namespace std ;


bool is_asc_sorted(const int array[], const int max_size) {
    if (max_size == 1) {
        return true;
    }

    for (int index = 1; index < max_size; ++index) {
        if (array[index - 1] > array[index]) {
            return false;
        }
    }

    return true;
} // 기본 built-in 함수들이 snake-case를 사용하고 있기에 snake-case로 작성하였습니다.

bool is_dsc_sorted(const int array[], const int max_size) {
    if (max_size == 1) {
        return true;
    }

    for (int index = 1; index < max_size; ++index) {
        if (array[index - 1] < array[index]) {
            return false;
        }
    }

    return true;
} // 기본 built-in 함수들이 snake-case를 사용하고 있기에 snake-case로 작성하였습니다.

bool readArray(int array[], const int array_size, const int array_order_type) {
    for (int index = 0; index < array_size; ++index) {
        std::cin >> array[index];
    }

    if (array_order_type == 1) {
        return is_asc_sorted(array, array_size);
    }

    return is_dsc_sorted(array, array_size);
}

bool getMinMax(const int asc_array[], const int dsc_array[], const int array_size, int &find_min, int &find_max) {

    const int arr1_min = asc_array[0];
    const int arr1_max = asc_array[array_size - 1];

    const int arr2_min = dsc_array[array_size - 1];
    const int arr2_max = dsc_array[0];

    find_min = arr1_min <= arr2_min ? arr1_min : arr2_min;
    find_max = arr1_max >= arr2_max ? arr1_max : arr2_max;

    return find_min != find_max;
}