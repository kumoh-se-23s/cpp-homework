//
// Created by apsode on 26. 11. 25..
//

#ifndef CPP_HOMEWORK_LIFELONG_H
#define CPP_HOMEWORK_LIFELONG_H
#include "AdStudent.h"

class LifeLong : public AdStudent<LifeLong> {
    MyString m_uid;

    MyString m_tuition;

    public:
        MyString tuition();

    private:
        MyString get_uid();
};

#endif //CPP_HOMEWORK_LIFELONG_H
