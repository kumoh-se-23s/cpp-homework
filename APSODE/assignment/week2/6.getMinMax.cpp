//
// Created by leegu on 26. 9. 16..
//
#include <iostream>
using namespace std ;


bool is_asc_sorted(const int array[], int max_size) {
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

bool is_dsc_sorted(const int array[], int max_size) {
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

bool readArray(int array[], int array_size, int array_order_type) {
    std::cin >> array[0] >> array[1] >> array[2] >> array[3] >> array[4];

    if (array_order_type == 1) {
        return is_asc_sorted(array, array_size);
    }

    return is_dsc_sorted(array, array_size);
}

bool getMinMax(const int asc_array[], const int dsc_array[], int array_size, int &find_min, int &find_max) {
    int arr1_min = asc_array[0], arr1_max = asc_array[array_size - 1];
    int arr2_min = dsc_array[array_size - 1], arr2_max = dsc_array[0];

    find_min = arr1_min <= arr2_min ? arr1_min : arr2_min;
    find_max = arr1_max >= arr2_max ? arr1_max : arr2_max;

    return find_min != find_max;
}

int main() {
    const int SIZE = 5, ASCENDING = 1, DESCENDING = 2;
    int cnt, arr1[SIZE], arr2[SIZE], min = 0, max = 0;
    cin >> cnt;
    for (int i = 0 ; i < cnt ; ++i) {
        if ( readArray(arr1, SIZE, ASCENDING) && readArray(arr2, SIZE, DESCENDING) )
            if ( getMinMax(arr1, arr2, SIZE, min, max) )
                cout << "Min : " << min << ", Max : " << max << "\n";
            else
                cout << "두 배열의 모든 값이 동일함" << min << "\n";
            
        else cout << "[Error] Unsorted input\n";
    }
    return 0;
}