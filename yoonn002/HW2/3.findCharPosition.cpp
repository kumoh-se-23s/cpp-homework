/*
1. 문자열을 입력 받음과 동시에 배열에 하나씩 집어 넣기 
2. 배열에 집어넣으면서 유니코드를 통해 해당되는 배열 칸에 집어넣기
*/


#include <iostream>
using namespace std;

void cntChar(char str[], int cnt[]);
void printCnt(int cnt[]);

int main(){
    const int MAX_SIZE = 101;
    char inStr[MAX_SIZE];
    int charPosition[26] = {};


    cin >> inStr; //바로 집어넣기 가능    
    cntChar(inStr, charPosition);
    printCnt(charPosition);

    return 0;

}


void cntChar(char str[], int cnt[]){
    for(int i = 0; str[i] != '\0'; i++){
        if (cnt[(int)str[i]-97]==0){
            cnt[(int)str[i]-97] = i+1;
        }
        
    }
}

void printCnt(int cnt[]){
    for(int i = 0; i < 26; i++){
        cout << cnt[i] << " ";
    }
    cout << endl;
}
