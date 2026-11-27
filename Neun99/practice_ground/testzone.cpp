#include <iostream>
#include <string>

using namespace std;

void test();

int main() {
  test();
}

void test() {
  cout << "AA" << endl;
  string a;
  cin >> a;
  cout << a << endl;
  getline(cin, a);
  cout << a << endl;
}