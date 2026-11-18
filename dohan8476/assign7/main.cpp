#include "StudentListManager.h"
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <filename>" << endl;
        return 1;
    }

    StudentListManger manager(argv[1]);
    manager.run();

    return 0;
}