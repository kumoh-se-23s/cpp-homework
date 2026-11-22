#pragma once

#include <fstream>
#include "MyString.h"

using namespace std;

class MyStringFileIO {
    const char *filePath = nullptr;
    ifstream in;
    ofstream out;

public:
    MyStringFileIO();

    MyStringFileIO(const char *path);

    ~MyStringFileIO();

    bool openRead();

    bool openWrite();

    void closeRead();

    void closeWrite();

    ifstream& getIn();

    ofstream& getOut();

    bool readLine(MyString &line);

    void writeLine(const MyString &line);

    bool isPath() const;
};

