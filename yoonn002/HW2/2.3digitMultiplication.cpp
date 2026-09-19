#include <iostream>
using namespace std;

void digitSave(int, int arr[],int );

int main(){

    const int MAX_SIZE = 4;
    int A, B;
    int arr[MAX_SIZE];
    
    cin >> A >> B;

    digitSave(MAX_SIZE, arr, B);
    for (int i = 0; i < MAX_SIZE; i++){
        cout << A*arr[i] << endl;
    }

    return 0;

}

void digitSave(int MAX, int arr[], int B){
    arr[0] = B%10;
    arr[1] = (B/10)%10;
    arr[2] = B/100;
    arr[3] = B;
    
}