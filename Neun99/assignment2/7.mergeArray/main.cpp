#include <iostream>

#include "MyArray.h"

using namespace std;

int main() {
    MyArray1 arr1, arr2 ;
    int cnt ;
    cin >> cnt ;
    for (int i = 0 ; i < cnt ; ++i) {
        if ( arr1.readAscSorted() && arr2.readAscSorted() ){
            MyArray2 resultArray = arr1.merge(arr2) ;
            resultArray.print() ;
        }
    }
    return 0 ;
}
