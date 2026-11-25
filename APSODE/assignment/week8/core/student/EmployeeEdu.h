//
// Created by apsode on 26. 11. 25..
//

#ifndef CPP_HOMEWORK_EMPLOYEEEDU_H
#define CPP_HOMEWORK_EMPLOYEEEDU_H
#include "AdStudent.h"

class EmployeeEdu : public AdStudent<EmployeeEdu> {
    MyString m_uid;

    MyString m_company;
    MyString m_position;

    public:
        MyString company();

        MyString position();

    private:
        MyString get_uid();
};

#endif //CPP_HOMEWORK_EMPLOYEEEDU_H
