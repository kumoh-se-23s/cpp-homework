#ifndef ADSTUDENT_H
#define ADSTUDENT_H
#include <istream>
#include <ostream>
#include "../../base/string/MyString.h"
#include "visitor/StudentVisitor.h"

class AdStudent {
    public:
        virtual ~AdStudent();


        [[nodiscard]] virtual const MyString& uid() const = 0;

        virtual void accept(StudentVisitor &visitor) const = 0;

        virtual void read_details(std::istream &input) = 0;

        virtual void write(std::ostream &output) const = 0;

        virtual void release_uid() const;


        [[nodiscard]] const MyString& name() const;

        [[nodiscard]] const MyString& course() const;

        [[nodiscard]] const MyString& phone_number() const;


        bool operator==(const AdStudent &other) const;

        bool operator==(const MyString &key) const;

        bool operator!=(const AdStudent &other) const;

        bool operator>(const AdStudent &other) const;

        bool operator>=(const AdStudent &other) const;

        bool operator<(const AdStudent &other) const;

        bool operator<=(const AdStudent &other) const;

    protected:
        MyString m_name;
        MyString m_course;
        MyString m_phone_number;

        explicit AdStudent(const MyString &name);
};

#endif //ADSTUDENT_H
