//
// Created by apsode on 26. 11. 25..
//

#ifndef CPP_HOMEWORK_GIFTED_H
#define CPP_HOMEWORK_GIFTED_H
#include "AdStudent.h"

class Gifted : public AdStudent<Gifted> {
    MyString m_uid;

    MyString m_middle_school;

    public:
        MyString middle_school();

    private:
        MyString get_uid();
};

#endif //CPP_HOMEWORK_GIFTED_H
