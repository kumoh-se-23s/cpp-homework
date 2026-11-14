//
// Created by apsode on 26. 11. 14..
//

#ifndef CPP_HOMEWORK_STUDENT_H
#define CPP_HOMEWORK_STUDENT_H

#include "../base/MyString.h"

class Student {
    static constexpr auto DEFAULT_NAME = "default name";
    static constexpr auto DEFAULT_UID = "default uid";
    static constexpr auto DEFAULT_GID = "default gid";

    MyString m_name;
    MyString m_uid;
    MyString m_gid;

    public:
        Student();

        Student(const MyString &name, const MyString &uid, const MyString &gid);

        Student(const char* name, const char* uid, const char* gid);

        [[nodiscard]] MyString name() const;

        [[nodiscard]] MyString uid() const;

        [[nodiscard]] MyString gid() const;


        // 일단 mutable하게 제공. 이후 immutable하게 수정 고려
        MyString set_name(const MyString& new_name);

        MyString set_uid(const MyString& new_uid);

        MyString set_gid(const MyString& new_gid);

    private:
        // 필요하려나..?
};

#endif //CPP_HOMEWORK_STUDENT_H


