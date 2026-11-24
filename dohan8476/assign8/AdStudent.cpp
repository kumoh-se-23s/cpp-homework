#include "AdStudent.h"

AdStudent::~AdStudent() = default;

bool AdStudent::operator<(const AdStudent &other) const {
    if (this->StudentName != other.StudentName) {
        return this->StudentName < other.StudentName;
    }
    return this->phoneNumber < other.phoneNumber;
}

bool AdStudent::operator<=(const AdStudent &other) const {
    if (this->StudentName != other.StudentName) {
        return this->StudentName <= other.StudentName;
    }
    return this->phoneNumber <= other.phoneNumber;
}

bool AdStudent::operator==(const AdStudent &other) const {
    return this->StudentName == other.StudentName;
}

//virtual -> override
void AdStudent::input() {

}
MyString AdStudent::toString() const {
    return 0;
}

MyString AdStudent::toFileFormatString() const {
    return 0;
}
