#include "OldMyStringFileIO.h"
#include <fstream>


OldMyStringFileIO::OldMyStringFileIO() : OldMyStringFileIO(nullptr) {}

OldMyStringFileIO::OldMyStringFileIO(const char *path) {
    filePath = path;
}

OldMyStringFileIO::~OldMyStringFileIO() {
    closeRead();
    closeWrite();
}

bool OldMyStringFileIO::openRead() {
    if (!isPath()) {
        return false;
    }
    closeRead();
    in.open(filePath);
    return in.is_open();
}

bool OldMyStringFileIO::openWrite() {
    if (!isPath()) {
        return false;
    }
    closeWrite();
    out.open(filePath);
    return out.is_open();
}

void OldMyStringFileIO::closeRead() {
    if (in.is_open()) {
        in.close();
    }
}

void OldMyStringFileIO::closeWrite() {
    if (out.is_open()) {
        out.close();
    }
}

ifstream& OldMyStringFileIO::getIn() {
    return in;
}

ofstream& OldMyStringFileIO::getOut() {
    return out;
}

bool OldMyStringFileIO::readLine(OOldMyString &line) {
    if (!in.is_open()) {
        return false;
    }
    getline(in, line);
    return !in.fail();
}

void OldMyStringFileIO::writeLine(const OOldMyString &line) {
    if (out.is_open()) {
        out << line << '\n';
    }
}

bool OldMyStringFileIO::isPath() const {
    return filePath != nullptr && filePath[0] != '\0';
}
