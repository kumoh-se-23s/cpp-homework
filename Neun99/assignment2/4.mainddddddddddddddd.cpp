//제출본에서 제외할것!!!!!!!!!!!!
#include <iostream>

using namespace std ;

bool readAscSortedArray(int [], int) ;
void mergeArray(int [], int [], int[], int) ;
void printArray(int [], int) ;

int main() {
    const int SIZE = 5 ;
    int cnt, arr1[SIZE], arr2[SIZE], resultArr[SIZE*2];
    cin >> cnt ;
    for (int i = 0 ; i < cnt ; ++i) {
        if ( readAscSortedArray(arr1, SIZE) && readAscSortedArray(arr2, SIZE) ) {
            mergeArray(arr1, arr2, resultArr, SIZE) ;
            printArray(resultArr, SIZE*2) ;
        }
    }
    return 0 ;
}

//오름차순 받고 검증
bool readAscSortedArray(int arr[], int size) {
    for (int idx = 0; idx < size; idx++) {
        cin >> arr[idx];
    }

    for (int idx = 0; idx < size - 1; idx++) {
        if (arr[idx] > arr[idx+1]) {
            cout << "[Error] Unsorted input" << endl;
            return false;
        }
    }
    return true;
}

//병합
void mergeArray(int arr1[], int arr2[], int resultArr[], int size) {
    int arr1Idx = 0;
    int arr2Idx = 0;

    for (int idx = 0; idx < size * 2; idx++) {
        //이거 개선하기
        if (arr1Idx < size && arr2Idx < size) {
            if (arr1[arr1Idx] >= arr2[arr2Idx]) {
                resultArr[idx] = arr2[arr2Idx];
                arr2Idx++;
            } else {
                resultArr[idx] = arr1[arr1Idx];
                arr1Idx++;
            }
        } else if (arr2Idx < size) { //한쪽 배열 먼저 끝나면 남은 쪽 밀어넣기
            resultArr[idx] = arr2[arr2Idx];
            arr2Idx++;
        } else {
            resultArr[idx] = arr1[arr1Idx];
            arr1Idx++;
        }
    }
}

//출력
void printArray(int arr[], int size) {
    cout << arr[0];
    for (int idx = 1; idx < size; idx++) {
        cout << " " << arr[idx];
    }
    cout << endl;
}