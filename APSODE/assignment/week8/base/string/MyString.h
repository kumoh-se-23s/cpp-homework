//
// Created by apsode on 26. 11. 2..
//

#ifndef CPP_HOMEWORK_WEEK6_MYSTRING_H
#define CPP_HOMEWORK_WEEK6_MYSTRING_H
#include <iostream>

// Sequence<T> -> CharSequence -> MyString을 포기했습니다.
class MyString {
    static constexpr int TAIL = 1;
    static constexpr int DEFAULT_CAPACITY = 8;
    static constexpr int GROWTH_FACTOR = 2;

    constexpr static int HASH_MODULAR = 1e9 + 7;
    constexpr static int HASH_BASE = 257;

    char *container;
    int m_capacity;
    int m_size;

    public:
        MyString();

        MyString(const MyString &other);

        MyString(const char *other);

        MyString(const char *elements, int size);

        ~MyString();


        int length() const; // 과제 양식에 따라 camelCase

        bool isEmpty() const; // 과제 양식에 따라 camelCase

        int getCapacity() const; // 과제 양식에 따라 camelCase

        const char& at(int index) const;

        char& at(int index);

        const char& operator[](int index) const;

        char& operator[](int index);


        int find(const MyString &subStr, int position = 0) const;

        int find(const char *pattern, int start) const;

        MyString substr(int position, int length) const;


        MyString& append(char character);

        MyString& append(const char *chars);

        MyString& append(const char *elements, int amount);

        MyString& append(const MyString &str2nd);

        MyString& insert(int position, const char &element);

        MyString& insert(int position, const char *chars);

        MyString& insert(int position, const char *elements, int amount);

        MyString& insert(int position, const MyString &subStr);

        MyString& replace(int position, int length, const MyString &subStr);

        MyString& replace(int position, int length, const char *subStr);

        MyString& erase(int start, int amount);

        void clear();


        int compare(const MyString &other) const;

        int compare(const char *chars) const;

        int compare(const char *chars, int length) const;

        bool equals(const MyString &other) const;

        bool operator==(const MyString &other) const {
            return this->compare(other) == 0;
        }

        bool operator!=(const MyString &other) const {
            return this->compare(other) != 0;
        }

        bool operator>(const MyString &other) const {
            return this->compare(other) > 0;
        }

        bool operator<(const MyString &other) const {
            return this->compare(other) < 0;
        }

        bool operator>=(const MyString &other) const {
            return this->compare(other) >= 0;
        }

        bool operator<=(const MyString &other) const {
            return this->compare(other) <= 0;
        }

        bool operator==(const char *other) const {
            return this->compare(other) == 0;
        }

        bool operator!=(const char *other) const {
            return this->compare(other) != 0;
        }

        bool operator>(const char *other) const {
            return this->compare(other) > 0;
        }

        bool operator<(const char *other) const {
            return this->compare(other) < 0;
        }

        bool operator>=(const char *other) const {
            return this->compare(other) >= 0;
        }

        bool operator<=(const char *other) const {
            return this->compare(other) <= 0;
        }


        MyString& operator=(const MyString &other);

        MyString& operator=(const char *chars);

        MyString operator+(const MyString &str2nd) const;

        MyString operator+(const char *chars) const;

        MyString& operator+=(const MyString &str2nd);

        MyString& operator+=(const char *chars);

    private:
        bool is_valid_index(int index) const;

        bool is_alias_container(const char *maybe_this) const;

        static void require_non_null(const char *nullable);

        static void require_valid_amount(int amount);


        static void swap(MyString &string1, MyString &string2) {
            char *temp_container = string1.container;
            string1.container = string2.container;
            string2.container = temp_container;

            int temp_size = string1.m_size;
            string1.m_size = string2.m_size;
            string2.m_size = temp_size;

            int temp_capacity = string1.m_capacity;
            string1.m_capacity = string2.m_capacity;
            string2.m_capacity = temp_capacity;
        }

        static void swap(MyString &string1, MyString &&string2) {
            return MyString::swap(string1, string2);
        }

        static char* create_empty_container(int new_capacity) {
            return new char[new_capacity + TAIL]();
        }

        constexpr static int calc_new_capacity(int amount) {
            if (amount < DEFAULT_CAPACITY) {
                return DEFAULT_CAPACITY;
            }

            return (amount + DEFAULT_CAPACITY - 1) / DEFAULT_CAPACITY * DEFAULT_CAPACITY;
        }

        static void copy_array(const char *src, int src_capacity, char *dest, int dest_capacity) {
            for (int index = 0; index < src_capacity && index < dest_capacity; ++index) {
                dest[index] = src[index];
            }
        }

        bool need_shrink() const {
            return this->m_capacity > DEFAULT_CAPACITY && this->m_size <= this->m_capacity / 4;
        }

        bool need_extend(int size_delta = 1) const {
            return this->m_size + size_delta > this->m_capacity;
        }

        void shrink();

        void extend(int required);

        void fill_tail();


        static int count_chars(const char *chars);

        int rabin_karp(const MyString &pattern, int start) const;

        static bool is_same(const MyString &target, const MyString &pattern, int pattern_start);
};

std::ostream& operator<<(std::ostream &output_stream, const MyString &string);

std::istream& operator>>(std::istream &input_stream, MyString &string);

std::istream& getline(std::istream &input_stream, MyString &string, char delim = '\n');

int stoi(const MyString &string, int position = 0, int base = 10);

int stoi(const char* chars, int position = 0, int base = 10);

#endif //CPP_HOMEWORK_WEEK6_MYSTRING_H
