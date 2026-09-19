#include <iostream>
using namespace std;

void convertBinary(int num, int& stdNum, bool d[]);
void printArrNum(int& stdNum, bool a[]);


int main(){
    int cnt;

    cin >> cnt;
    for (int i = 0; i < cnt; i++){
        int num;
        int stdNum;

        const int MAX_SIZE = 32; //꼬옥 const 붙이기
        bool decimal[MAX_SIZE];

        cin >> num;
        convertBinary(num, stdNum, decimal);
        printArrNum(stdNum, decimal);

    }

    return 0;
}


void convertBinary(int num, int& stdNum, bool d[]){
    int remain;

    stdNum = 0;
    while ( num ){
        
        remain = num % 2;
        num /= 2;

        d[stdNum] = remain;

        stdNum++;

    }
}

void printArrNum(int& stdNum, bool a[]){
    
    bool isFirst = 1;

    for (int i = stdNum-1, j = 0; i >= 0; i--, j++){

        if (isFirst){
            j += 4 - stdNum%4;
            isFirst = 0;
        }

        else if (j%4 == 0){
            cout << " ";
        }
        
        cout << a[i] ;
        
    }
    cout << endl;
}



