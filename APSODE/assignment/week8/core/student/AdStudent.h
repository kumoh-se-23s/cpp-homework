#ifndef ADSTUDENT_H
#define ADSTUDENT_H
#include "../../string/MyString.h"

template<typename Derived>
class AdStudent {
    public:
        MyString uid();

        MyString name();

        MyString course();

        MyString phone_number();


        bool operator==(const AdStudent &other) const;

        bool operator!=(const AdStudent &other) const;

        bool operator>(const AdStudent &other) const;

        bool operator>=(const AdStudent &other) const;

        bool operator<(const AdStudent &other) const;

        bool operator<=(const AdStudent &other) const;

    protected:
        MyString m_name;
        MyString m_course;
        MyString m_phone_number;
};


#include "AdStudent.tpp"

#endif //ADSTUDENT_H
