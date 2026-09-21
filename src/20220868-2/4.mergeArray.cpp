
#include <iostream>

bool readAscSortedArray(int arr[], int size){
    using namespace std;
    int previousInput = 0;

    for(int i = 0; i < size; ++i){
        cin >> arr[i];
        if(arr[i] < previousInput) {
            return false;
        }
        previousInput = arr[i];
    }
    return true;
}

void mergeArray(int a[], int b[], int result[], int size){
    using namespace std;
    int aLoc = 0;
    int bLoc = 0;
    for(int loc = 0; loc < size * 2; ++loc){

        if(bLoc == size || (aLoc < size && a[aLoc] < b[bLoc])){
            result[loc] = a[aLoc];
            ++aLoc;
        }else{
            result[loc] = b[bLoc];
            ++bLoc;
        }
    }
}

void printArray(int arr[], int size){
    using namespace std;

    for(int i = 0; i < size; ++i){
        cout << arr[i] << " ";
    }
    cout << endl;
}
