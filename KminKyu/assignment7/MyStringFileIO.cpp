// #include "MyStringFileIO.h"
// #include "MyString.h"
//
// MyStringFileIO::MyStringFileIO(const char *filePath) : filePath(filePath) {}
//
// MyStringFileIO::~MyStringFileIO() {
//     closeAll();
// }
//
// bool MyStringFileIO::openInput() {
//     closeInput();
//     inFile.open(filePath);
//     return inFile.is_open();
// }
//
// bool MyStringFileIO::openOutput() {
//     closeOutput();
//     // outFile.open(filePath.c_str());
//     return outFile.is_open();
// }
//
// bool MyStringFileIO::readLine(char *buffer, int bufferSize) {
//     if (!inFile.is_open()) {
//         return false;
//     }
//     if (!inFile.getline(buffer, bufferSize)) {
//         return false;
//     }
//     // std::size_t len = std::strlen(buffer);
//     if (len > 0 && buffer[len - 1] == '\r') {
//         buffer[len - 1] = '\0';
//     }
//     return true;
// }
//
// void MyStringFileIO::write(const MyString &content) {
//     if (outFile.is_open()) {
//         outFile << content;
//     }
// }
//
// void MyStringFileIO::closeInput() {
//     if (inFile.is_open()) {
//         inFile.close();
//     }
// }
//
// void MyStringFileIO::closeOutput() {
//     if (outFile.is_open()) {
//         outFile.close();
//     }
// }
//
// void MyStringFil#include "Concept.h"eIO::closeAll() {
//     closeInput();
//     closeOutput();
// }