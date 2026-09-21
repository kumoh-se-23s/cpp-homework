
#include <iostream>



void printCharPositions(const char *inStr){
    using namespace std;
    constexpr int ALPHABETS = 26;

    int loc[ALPHABETS]{0};

    for(int i = 0; inStr[i] != '\0'; ++i){
        int &ref = loc[inStr[i]-'a'];
        if(ref == 0) ref = i + 1;
    }

    for(int i = 0; i < ALPHABETS; ++i){
        cout << loc[i] << " "; 
    }
    cout << endl;
}



int main(){
    using namespace std;

    char inStr[101]{'\0'};
    cin >> inStr; //단순 예시. 원래는 동적 할당을 사용합니다

    printCharPositions(inStr);


    return 0;
}