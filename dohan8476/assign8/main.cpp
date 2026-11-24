#include "App.h"
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <filename>" << endl;
        return 1;
    }

    // App app(argv[1]);
    // app.run();

    return 0;
}