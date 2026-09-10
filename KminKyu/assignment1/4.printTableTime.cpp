#include<iostream>

using namespace std;

int main() {
    int testCase;
    cin >> testCase;

    int maxNum, maxDan, maxLine;
    for (int caseNum = 0; caseNum < testCase; ++caseNum) {
        cin >> maxNum >> maxDan >> maxLine;
        for (int nowNum = 0; nowNum < maxNum; nowNum += maxLine) {
            for (int nowDan = 1; nowDan <= maxDan; ++nowDan) {
                for (int nowLine = 1; nowLine + nowNum <= maxNum && nowLine <= maxLine; ++nowLine) {
                    cout << nowLine + nowNum << " * " << nowDan << " = " << ((nowNum + nowLine) * nowDan);
                    cout << "   ";
                }
                cout << endl;    
            }
            cout << "\n";        
            
        }

    }   
    
    return 0;
}