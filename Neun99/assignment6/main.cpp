#include "MyString.h"

int main() {
    MyString str("0123456789");

    cout << str.substr(-1, 3) << "." << endl; // 제일 뒤글자부터 3글자인 "789" 출력
    cout << str.substr(-2, 2) << "." <<endl; // 뒤 두번째 글자인 '8'부터 두글자 앞까지인 "78" 출력
    cout << str.substr(-5, 4) << "." <<endl; // 뒤에서 다섯번째 글자인 '5'부터 4글자인 "2345" 출력
    cout << str.substr(-5, 10) << "." <<endl; // 범위를 넘어서면 문자열 처음까지만. "012345" 출력
    cout << str.substr(-15, 3) << "." <<endl; // 불가능한 위치이므로 빈문자열 "" 출력
    cout << str.substr(20, 3) << "." <<endl; // 예외처리 하지 말고 빈문자열 "" 출력
}