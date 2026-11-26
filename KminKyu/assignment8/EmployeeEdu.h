#pragma once
#include "AdStudent.h"

class EmployeeEdu : public AdStudent {

    MyString studentId;
    MyString companyName;
    MyString position;

    static int LAST_STUDENT_ID;
};

int EmployeeEdu::LAST_STUDENT_ID = 0;


