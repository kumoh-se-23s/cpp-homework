#include<iostream>
#include "Date.h"

using namespace std;

int main() {
    Date date;
    date.read();
    cout << date.getString() << endl;
    return 0;
}