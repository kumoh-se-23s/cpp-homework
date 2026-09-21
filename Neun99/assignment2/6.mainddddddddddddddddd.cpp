#include <iostream>

using namespace std ;

bool readArray(int [], int, int) ; 
bool getMinMax(int [], int [], int, int&, int&) ;

int main() {
    const int SIZE = 5, ASCENDING = 1, DESCENDING = 2 ;
    int cnt, arr1[SIZE], arr2[SIZE], min=0, max=0 ;
    cin >> cnt ;
    for (int i = 0 ; i < cnt ; ++i) {
        if (readArray(arr1, SIZE, ASCENDING) && readArray(arr2, SIZE, DESCENDING))
            if (getMinMax(arr1, arr2, SIZE, min, max))
                cout << "Min : " << min << ", Max : " << max << "\n" ;
            else
                cout << "SAME MIN AND MAX" << min << "\n" ;
        else cout << "[Error] Unsorted input\n" ;
    }
    return 0 ;
}

bool readArray(int arr[], int size, int type) {
    //입력받기
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

//모두 동일하면 false 리턴
bool getMinMax(int arr1[], int arr2[], int size, int& min, int& max) {  
    //arr1 최소최대 찾기
    if(arr1[0] < arr1[size - 1]) { //오름차순배열이면
        min = arr1[0];
        max = arr1[size - 1];
    } else { //내림차순배열이면
        min = arr1[size - 1];
        max = arr1[0];
    }

    //전체 최소최대 찾기
    if(arr2[0] < arr2[size - 1]) { //오름차순배열이면
        if(arr2[0] < min) 
            min = arr2[0];
        if(arr2[size - 1] > max)
            max = arr2[size-1];
    } else { //내림차순배열이면
        if(arr2[size - 1] < min)
            min = arr2[size - 1];
        if(arr2[0] > max)
            max = arr2[0];
    }
    if(min == max)
        return false;
    return true;
}