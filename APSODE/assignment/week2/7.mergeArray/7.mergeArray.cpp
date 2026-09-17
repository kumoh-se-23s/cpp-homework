
#include <iostream>
#include "7.mergeArray.hpp"


bool MyArray1::is_asc_sorted() const {
    for (int index = 1; index < DEFAULT_ARRAY_CAPACITY; ++index) {
        if (this->array[index - 1] > this->array[index]) {
            return false;
        }
    }

    return true;
}

bool MyArray1::readAscSorted() {
    for (int index = 0; index < DEFAULT_ARRAY_CAPACITY; ++index) {
        std::cin >> this->array[index];
    }

    return this->is_asc_sorted();
}

int & MyArray1::operator[](int index) {
    return this->array[index];
}

const int & MyArray1::operator[](const int index) const {
    return this->array[index];
}

MyArray2 MyArray1::merge(MyArray1 other_array) const {
    int arr1_index = 0;
    int arr2_index = 0;
    int result_array_index = 0;
    MyArray2 result_array = MyArray2();
    for (
        ;
        arr1_index < DEFAULT_ARRAY_CAPACITY && arr2_index < DEFAULT_ARRAY_CAPACITY;
        ++result_array_index
    ) {
        result_array[result_array_index] = this->array[arr1_index] < other_array[arr2_index]
                                        ? this->array[arr1_index++]
                                        : other_array[arr2_index++];
    }

    for (; arr1_index < DEFAULT_ARRAY_CAPACITY; ++arr1_index) result_array[result_array_index++] = this->array[arr1_index];
    for (; arr2_index < DEFAULT_ARRAY_CAPACITY; ++arr2_index) result_array[result_array_index++] = other_array[arr2_index];

    return result_array;
}

void MyArray2::print() const {
    for (int index = 0; index < DEFAULT_ARRAY_CAPACITY; ++index) {
        std::cout << this->array[index] << " ";
    }
    std::cout << std::endl;
}

int & MyArray2::operator[](const int index) {
    return this->array[index];
}

const int & MyArray2::operator[](const int index) const {
    return this->array[index];
}


int main() {
    MyArray1 arr1, arr2 ;
    int cnt ;
    std::cin >> cnt ;
    for (int i = 0 ; i < cnt ; ++i) {
        if ( arr1.readAscSorted() && arr2.readAscSorted() ){
            MyArray2 resultArray = arr1.merge(arr2) ;
            resultArray.print();
        }
        else std::cout << "[Error] Unsorted input\n" ;
    }
    return 0 ;
}

