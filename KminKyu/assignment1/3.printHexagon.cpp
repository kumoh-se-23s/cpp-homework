#include<iostream>

using namespace std;

int main() {
    int testCase;
    cin >> testCase;
    
    for (int caseNum = 0; caseNum < testCase; ++caseNum) {
        int maxHeight;
        cin >> maxHeight;

        for (int nowHeight = 0; nowHeight < maxHeight - 1; ++nowHeight) {
            
            for (int spaceCnt = 0; spaceCnt < maxHeight - nowHeight - 1; ++spaceCnt) {
                cout << " ";
            }
            
            for (int starCnt = 0; starCnt < maxHeight + nowHeight * 2; ++starCnt) {
                cout << "*";
            }
            cout << endl;
            
        }
        
        for (int nowHeight = 0; nowHeight < maxHeight; ++nowHeight) {
            
            for (int starCnt = 0; starCnt < 2 * maxHeight + (maxHeight -2); starCnt++) {
                cout << "*";
            }
            cout << endl;

        }

        for (int nowHeight = 0; nowHeight < maxHeight - 1; ++nowHeight)
        {
            for (int spaceCnt = 0; spaceCnt < nowHeight + 1; ++spaceCnt) {
                cout << " ";
            }
            
            for (int starCnt = 0; starCnt < 3 * maxHeight - 2 * nowHeight - 4; ++starCnt) {
                cout << "*";
            }
            cout << endl;
        }
    }   
    
    return 0;
}