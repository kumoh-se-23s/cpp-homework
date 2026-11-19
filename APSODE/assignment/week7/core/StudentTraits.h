#ifndef CPP_HOMEWORK_STUDENTTRAITS_H
#define CPP_HOMEWORK_STUDENTTRAITS_H

#include "Student.h"

class StudentTraits {
    static constexpr int ROW_KEY_LENGTH = 4;
    static constexpr auto NO_COLUMN = "-";

    public:
        StudentTraits() = delete;

        static int row_key(const Student &student);

        static MyString column_key(const Student &student);

        static bool has_column(const Student &student);

        static bool less(const Student &left, const Student &right);

        static bool same(const Student &left, const Student &right);
};

#endif //CPP_HOMEWORK_STUDENTTRAITS_H
