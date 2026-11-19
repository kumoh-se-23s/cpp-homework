#include <iostream>

#include "ManageCircleApp.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <file>\n";
        return 1;
    }

    ManageCircleApp app(argv[1]);
    app.run();

    return 0;
}
