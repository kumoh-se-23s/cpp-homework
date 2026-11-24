#pragma once

#include <fstream>
#include "OOldMyString.h"

using namespace std;

class OldMyStringFileIO {
    const char *filePath = nullptr;
    ifstream in;
    ofstream out;

public:
    OldMyStringFileIO();

    OldMyStringFileIO(const char *path);

    ~OldMyStringFileIO();

    bool openRead();

    bool openWrite();

    void closeRead();

    void closeWrite();

    ifstream& getIn();

    ofstream& getOut();

    bool readLine(OOldMyString &line);

    void writeLine(const OOldMyString &line);

    bool isPath() const;
};

