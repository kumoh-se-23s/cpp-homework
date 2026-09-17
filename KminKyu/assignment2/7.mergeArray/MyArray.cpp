#include<iostream>
#include"MyArray.h"

using namespace std;

MyArray1::MyArray1() {
    nowSize = 0;
}

bool MyArray1::readAscSorted() {
    for (int arrSize = 0; arrSize < MAX_ARRAY_SIZE; ++arrSize) {
        int item;
        cin >> item;
        append(item);        
    }
    for (int arrSize = 0; arrSize < nowSize - 1; ++arrSize) {
        if (array[arrSize] > array[arrSize + 1]) {
            return false;
        }   
    }
    return true;
}

bool MyArray1::append(int item) {
    if (nowSize != MAX_ARRAY_SIZE) {
        array[nowSize++] = item;
        return true;
    } else {
        return false;
    }
}

void MyArray1::print() {
    for (int index = 0; index < nowSize; ++index) {
        if (index != 0) {
            cout << " ";
        }
        cout << array[index];
    }
    cout << endl;
}

int MyArray1::getNowSize() const {
    return nowSize;
}
int MyArray1::get(int index) const {
    return array[index];
}
MyArray2 MyArray1::merge(MyArray1 mergeableArray) const {
    int baseArrayIndex = 0, mergeableArrayIndex = 0;
    MyArray2 mergedArray;
    while (baseArrayIndex < nowSize && mergeableArrayIndex < mergeableArray.getNowSize()) {

        int baseArrayValue = get(baseArrayIndex), mergeArrayValue = mergeableArray.get(mergeableArrayIndex);
        
        if (baseArrayValue < mergeArrayValue) {
            if (mergedArray.append(baseArrayValue)) {
                ++baseArrayIndex;
            }
        } else {
            if (mergedArray.append(mergeArrayValue)) {
                ++mergeableArrayIndex;
            }
        }
    }
    while (baseArrayIndex < nowSize) {
       if (mergedArray.append(get(baseArrayIndex))) {
                ++baseArrayIndex;
            }
    }
    while (mergeableArrayIndex < mergeableArray.getNowSize()) {
        if (mergedArray.append(mergeableArray.get(mergeableArrayIndex))) {
            ++mergeableArrayIndex;
        }
    }
    return mergedArray;
}


MyArray2::MyArray2() {
    nowSize = 0;
}

bool MyArray2::readAscSorted() { //사용하지는 않지만 임시로 구현
    for (int arrSize = 0; arrSize < MAX_ARRAY_SIZE; ++arrSize) {
        int item;
        cin >> item;
        append(item);
    }
    for (int arrSize = 0; arrSize < nowSize - 1; ++arrSize) {
        if (array[arrSize] > array[arrSize + 1]) {
            return false;
        }   
    }
    return true;
}

bool MyArray2::append(int item) {
    if (nowSize != MAX_ARRAY_SIZE) {
        array[nowSize++] = item;
        return true;
    } else {
        return false;
    }
}

void MyArray2::print() {
    for (int index = 0; index < nowSize; ++index) {
        if (index != 0) {
            cout << " ";
        }
        cout << array[index];
    }
    cout << endl;
}

int MyArray2::getNowSize() const {
    return nowSize;
}
int MyArray2::get(int index) const {
    return array[index];
}

int main() 
{
    MyArray1 arr1, arr2 ;
    int cnt ;
    cin >> cnt ;
    for (int i = 0 ; i < cnt ; ++i) {
        if ( arr1.readAscSorted() && arr2.readAscSorted() ){
            MyArray2 resultArray = arr1.merge(arr2) ;
            resultArray.print() ;
        } else cout << "[Error] Unsorted input\n";
    }
    return 0 ;
}