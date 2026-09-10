#include<iostream>

using namespace std;

int main() {
    int testCase;
    cin >> testCase;

    int maxMultiNum, maxDan, maxLine;
    for (int caseNum = 0; caseNum < testCase; ++caseNum) {
        cin >> maxDan >> maxMultiNum >> maxLine;
        for (int nowDan = 0; nowDan < maxDan; nowDan += maxLine) {
            for (int nowMultiNum = 1; nowMultiNum <= maxMultiNum; ++nowMultiNum) {
                for (int nowLine = 1; nowLine + nowDan <= maxDan && nowLine <= maxLine; ++nowLine) {
                    cout << nowLine + nowDan << " * " << nowMultiNum << " = " << ((nowDan + nowLine) * nowMultiNum);
                    cout << "   ";
                }
                cout << endl;    
            }
            cout << "\n";        
            
        }

    }   
    
    return 0;
}