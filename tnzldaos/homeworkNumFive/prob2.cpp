#include <iostream>
using namespace std;

void myCopy(int* arr1, int* arr2, int size) {
    int arr2Index = 0;

    for(int* index = &arr1[0] ; index < &arr1[size] ; index++) {
        arr2[arr2Index] = *index;
        arr2Index++; 
    }
}

void print1DArr(int* arr, int size) {
    for(int* index = &arr[0] ; index < &arr[size] ; index++) {
        cout << *index << " ";
    }
    cout << endl;
}

int main()
{
    const int MAX = 5;
    int arr1[MAX] = {1,2,3,4,5}, arr2[MAX], *p;
    
    myCopy(arr1,arr2,MAX);
    p = new int[MAX];
    myCopy(arr1, p , MAX);

    p[0] = 99;
    print1DArr(arr1,MAX);
    print1DArr(arr2,MAX);
    print1DArr(p,MAX);

    return 0;
}
