#include<iostream>

using namespace std;

int main() {
    int testCase;
    cin >> testCase;

    for (int caseNum = 0; caseNum < testCase; ++caseNum) {
        int maxMultiNum, maxDan, maxLine;
        cin >> maxDan >> maxMultiNum >> maxLine;
        for (int nowDan = 0; nowDan < maxDan; nowDan += maxLine) {
            for (int nowMultiNum = 1; nowMultiNum <= maxMultiNum; ++nowMultiNum) {
                for (int nowLine = 1; nowLine + nowDan <= maxDan && nowLine <= maxLine; ++nowLine) {
                    printf("%2d * %2d = %4d", nowLine + nowDan, nowMultiNum, (nowDan + nowLine) * nowMultiNum);
                    // cout << nowLine + nowDan << " * " << nowMultiNum << " = " << ((nowDan + nowLine) * nowMultiNum);
                    cout << "   ";
                }
                cout << endl;    
            }
            cout << "\n";        
            
        }

    }   
    
    return 0;
}