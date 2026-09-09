#include <iostream>

using namespace std;

void swap(int &a, int &b);
int main() {
   int stdScore1, stdScore2;
   cout << "학생 두명의 점수를 입력하세요 : ";
   cin >> stdScore1 >> stdScore2;

   swap(stdScore1, stdScore2);
   cout << "교환된 두개의 값은 : ";
   cout << stdScore1 << " " << stdScore2 << endl;

   return 0;

}
void swap(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
}
