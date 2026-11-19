#pragma once
#include <fstream>
#include "MyString.h"

using namespace std;
class MyStringFileIO {
    MyString filePath;
    std::ifstream inFile;
    std::ofstream outFile;
public:
    MyStringFileIO(const char *filePath);
    ~MyStringFileIO();


    bool openInput();
    bool openOutput();

    bool readLine(char *buffer, int bufferSize);
    void write(const MyString &studentStr);

    void closeInput();
    void closeOutput();
    void closeAll();
};


