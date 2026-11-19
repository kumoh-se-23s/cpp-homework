#include "StudentTraits.h"

int StudentTraits::row_key(const Student &student) {
    return stoi(student.uid().substr(0, ROW_KEY_LENGTH));
}

MyString StudentTraits::column_key(const Student &student) {
    return student.gid();
}

bool StudentTraits::has_column(const Student &student) {
    return student.gid() != NO_COLUMN;
}

bool StudentTraits::less(const Student &left, const Student &right) {
    return left.uid() < right.uid();
}

bool StudentTraits::same(const Student &left, const Student &right) {
    return left.uid() == right.uid();
}
