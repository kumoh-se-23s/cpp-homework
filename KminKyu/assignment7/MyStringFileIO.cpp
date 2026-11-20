#include "MyStringFileIO.h"
#include <fstream>


MyStringFileIO::MyStringFileIO() : MyStringFileIO(nullptr) {}

MyStringFileIO::MyStringFileIO(const char *path) {
    filePath = path;
}

MyStringFileIO::~MyStringFileIO() {
    closeRead();
    closeWrite();
}

bool MyStringFileIO::openRead() {
    if (!isPath()) {
        return false;
    }
    closeRead();
    in.open(filePath);
    return in.is_open();
}

bool MyStringFileIO::openWrite() {
    if (!isPath()) {
        return false;
    }
    closeWrite();
    out.open(filePath);
    return out.is_open();
}

void MyStringFileIO::closeRead() {
    if (in.is_open()) {
        in.close();
    }
}

void MyStringFileIO::closeWrite() {
    if (out.is_open()) {
        out.close();
    }
}

ifstream& MyStringFileIO::getIn() {
    return in;
}

ofstream& MyStringFileIO::getOut() {
    return out;
}

bool MyStringFileIO::readLine(MyString &line) {
    if (!in.is_open()) {
        return false;
    }
    getline(in, line);
    return !in.fail();
}

void MyStringFileIO::writeLine(const MyString &line) {
    if (out.is_open()) {
        out << line << '\n';
    }
}

bool MyStringFileIO::isPath() const {
    return filePath != nullptr && filePath[0] != '\0';
}
