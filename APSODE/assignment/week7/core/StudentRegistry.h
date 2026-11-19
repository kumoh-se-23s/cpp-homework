#ifndef CPP_HOMEWORK_STUDENTREGISTRY_H
#define CPP_HOMEWORK_STUDENTREGISTRY_H

#include <iostream>

#include "Student.h"
#include "StudentTraits.h"
#include "../base/MyString.h"
#include "../base/list/linked_list/MultiLinkedList.h"

class StudentRegistry {
    MultiLinkedList<Student, StudentTraits> m_list;

    public:
        StudentRegistry(int min_year, int max_year);

        StudentRegistry(const StudentRegistry &other) = delete;

        StudentRegistry& operator=(const StudentRegistry &other) = delete;


        static StudentRegistry* load(const char *path);

        bool save(const char *path) const;

        static bool read_student(std::istream &input_stream, Student &student);

        bool add(const Student &student);

        bool remove(const MyString &uid);

        void print(std::ostream &output_stream) const;

        [[nodiscard]] int student_count() const;

        [[nodiscard]] int team_count() const;

    private:
        void print_line(std::ostream &output_stream, const Student &student) const;

        static void trim(MyString &string);
};

#endif //CPP_HOMEWORK_STUDENTREGISTRY_H
