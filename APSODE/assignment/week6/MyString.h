//
// Created by apsode on 26. 11. 2..
//

#ifndef CPP_HOMEWORK_WEEK6_MYSTRING_H
#define CPP_HOMEWORK_WEEK6_MYSTRING_H
#include <iostream>


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


        int length() const;

        bool isEmpty() const;

        int getCapacity() const;

        int size() const;

        bool empty() const;

        int capacity() const;


        const char& at(int index) const;

        char& at(int index);

        const char& operator[](int index) const;

        char& operator[](int index);


        int find(const MyString &subStr, int pos = 0) const;

        int find(const char *pattern, int start) const;

        MyString substr(int pos, int len) const;


        MyString& append(char c);

        MyString& append(const char *chars);

        MyString& append(const char *elements, int amount);

        MyString& append(const MyString &str2nd);

        MyString& insert(int position, const char &element);

        MyString& insert(int position, const char *chars);

        MyString& insert(int position, const char *elements, int amount);

        MyString& insert(int pos, const MyString &subStr);

        MyString& replace(int pos, int len, const MyString &subStr);

        MyString& replace(int pos, int len, const char *subStr);

        MyString& erase(int start, int amount);

        void clear();


        int compare(const MyString &other) const;

        int compare(const char *chars) const;

        bool equals(const MyString &other) const;

        bool operator==(const MyString& other) const {
            return this->compare(other) == 0;
        }

        bool operator!=(const MyString& other) const {
            return this->compare(other) != 0;
        }

        bool operator>(const MyString& other) const {
            return this->compare(other) > 0;
        }

        bool operator<(const MyString& other) const {
            return this->compare(other) < 0;
        }

        bool operator>=(const MyString& other) const {
            return this->compare(other) >= 0;
        }

        bool operator<=(const MyString& other) const {
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

        void require_valid_index(int index) const;

        static void require_non_null(const char *nullable);

        static void require_valid_amount(int amount);


        static void swap(MyString &sequence1, MyString &sequence2) {
            char *temp_container = sequence1.container;
            sequence1.container = sequence2.container;
            sequence2.container = temp_container;

            int temp_size = sequence1.m_size;
            sequence1.m_size = sequence2.m_size;
            sequence2.m_size = temp_size;

            int temp_capacity = sequence1.m_capacity;
            sequence1.m_capacity = sequence2.m_capacity;
            sequence2.m_capacity = temp_capacity;
        }

        static void swap(MyString &sequence1, MyString &&sequence2) {
            return MyString::swap(sequence1, sequence2);
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

std::ostream& operator<<(std::ostream &output_stream, const MyString &char_sequence);

std::istream& operator>>(std::istream &is, MyString &str);

std::istream& getline(std::istream &is, MyString &str, char delim = '\n');

int stoi(const MyString &str, int pos = 0, int base = 10);

#endif //CPP_HOMEWORK_WEEK6_MYSTRING_H
