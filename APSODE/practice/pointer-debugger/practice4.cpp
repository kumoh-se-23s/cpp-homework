//
// Created by apsode on 26. 10. 7..
//
#include <iostream>
using namespace std;

class TestClass{
public :
    void print();
    void setNum(int n) { num = n ;  }
    void setArrItem(int i, int n) { arr[i] = n; }
private :
    int num = 33 ;
    static const int CAPACITY = 10;
    int arr[CAPACITY] = { 99,77 };
};

void TestClass::print() {
    printf("%d %d\n", num, arr[0]) ;
}

void test(TestClass &curT) {
    curT.setNum(88);
    curT.setArrItem(0, 44);
    curT.print();
}
int main()
{
    TestClass srcT ;
    srcT.print() ;
    test(srcT)  ;
    srcT.print();
    return 0 ;
}
