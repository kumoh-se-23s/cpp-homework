#include <iostream>
using namespace std;


bool readAscSortedArray(int arr[], int max){

    for (int i = 0; i < max; i++){
        cin >> arr[i];
    } 


    for (int i = 0; i < max-1; i++){
        if (arr[i] > arr[i+1]){
            return 0;
        } 
    }
    return 1;
}

void mergeArray(int arr1[], int arr2[], int resultArr[], int MAX){
    int a1 = 0;
    int a2 = 0;
    int cnt = 0;

    for (; cnt < MAX*2; cnt++){
        if (arr1[a1] < arr2[a2]) resultArr[cnt] = arr1[a1++];
        else resultArr[cnt] = arr2[a2++];
    }
    for (; a1 < MAX; a1++) resultArr[cnt] = arr1[a1++];
    for (; a2 < MAX; a2++) resultArr[cnt] = arr2[a2++];
}

void printArray(int resultArr[], int MAX){
    for (int i = 0; i < MAX*2; i++){
        cout << resultArr[i] << " ";
    }
    cout << endl;
}


int main(){
    int cnt; 

    cin >> cnt;
    for (int i = 0; i < cnt; i++){

        const int MAX = 5;
        int arr1[MAX], arr2[MAX], resultArr[MAX*2];

        cout << "arr1 : " << readAscSortedArray(arr1, MAX) << endl;
        cout << "arr2 : " << readAscSortedArray(arr2, MAX) << endl;

        mergeArray(arr1, arr2, resultArr, MAX);
        printArray(resultArr, MAX);
    }
}
