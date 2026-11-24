#pragma once

#include "OldMyStringFileIO.h"
#include "OldStudentController.h"

class OldApp {
    OldStudentController controller;
    OldMyStringFileIO fileIO;


public:
    OldApp();
    OldApp(int argc, char* argv[]);

    void start();
    int run(OOldMyString command);

private:
    bool loadFromFile();
    bool saveToFile();
    int insert();
    int remove();;
};

