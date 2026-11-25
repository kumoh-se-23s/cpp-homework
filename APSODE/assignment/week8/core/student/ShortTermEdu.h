//
// Created by apsode on 26. 11. 25..
//

#ifndef CPP_HOMEWORK_SHORTTERMEDU_H
#define CPP_HOMEWORK_SHORTTERMEDU_H
#include "AdStudent.h"
#include "MyString.h"

class ShortTermEdu : public AdStudent<ShortTermEdu> {
    MyString m_uid;

    MyString m_host_depart;

    public:
        MyString host_depart();

    private:
        MyString get_uid();
};

#endif //CPP_HOMEWORK_SHORTTERMEDU_H
