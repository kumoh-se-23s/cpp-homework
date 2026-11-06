//
// Created by apsode on 26. 11. 2..
//

#include "MyString.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <limits>
#include <stdexcept>

MyString::MyString()
    : container(new char[DEFAULT_CAPACITY + 1]()), m_capacity(DEFAULT_CAPACITY), m_size(0) {
}

MyString::MyString(const MyString &other)
    : m_capacity(other.m_capacity), m_size(other.m_size) {
    this->container = MyString::create_empty_container(other.m_capacity);
    MyString::copy_array(
        other.container, other.m_capacity,
        this->container, this->m_capacity
    );
}

MyString::MyString(const char *other)
    : MyString(other, count_chars(other)) {
}

MyString::MyString(const char *elements, int size)
    : m_capacity(MyString::calc_new_capacity(size)), m_size(size) {
    MyString::require_non_null(elements);
    MyString::require_valid_amount(size);
    this->container = MyString::create_empty_container(MyString::calc_new_capacity(this->m_size));
    MyString::copy_array(
        elements, size,
        this->container, this->m_capacity
    );
}

MyString::~MyString() {
    delete[] this->container;
}

int MyString::length() const {
    return this->m_size;
}

bool MyString::isEmpty() const {
    return this->m_size == 0;
}

int MyString::getCapacity() const {
    return this->m_capacity;
}

int MyString::size() const {
    return this->m_size;
}

bool MyString::empty() const {
    return this->m_size == 0;
}

int MyString::capacity() const {
    return this->m_capacity;
}

const char& MyString::at(int index) const {
    this->require_valid_index(index);
    return (*this)[index];
}

char& MyString::at(int index) {
    this->require_valid_index(index);
    return (*this)[index];
}

const char& MyString::operator[](int index) const {
    if (0 <= index && index < this->m_size) {
        return this->container[index];
    }
    return this->container[this->m_size];
}

char& MyString::operator[](int index) {
    if (0 <= index && index < this->m_size) {
        return this->container[index];
    }
    return this->container[this->m_size];
}

int MyString::find(const MyString &subStr, int pos) const {
    return this->rabin_karp(subStr, pos);
}

int MyString::find(const char *pattern, int start) const {
    return this->rabin_karp(MyString(pattern, count_chars(pattern)), start);
}

MyString MyString::substr(int pos, int len) const {
    MyString result = MyString();
    if (pos < 0) {
        // +00 +01 +02 +03 +04 +05 +06 +07 +08 +09
        // -10 -09 -08 -07 -06 -05 -04 -03 -02 -01
        // p:-3, l:3, cl:10 -> -3 + 10
        int temp = pos + this->m_size + 1;
        len = std::min(len, temp);
        pos = temp - len;
    } else {
        if (pos >= this->m_size && this->m_size > 0) {
            pos = this->m_size - 1;
        }

        if (len < 0) {
            len = this->m_size - pos;
        }
    }

    if (pos >= this->m_size || len <= 0) {
        return result;
    }

    for (int index = 0; index < len && pos + index < this->m_size; ++index) {
        result.append(this->container[pos + index]);
    }

    return result;
}

MyString& MyString::append(char c) {
    return this->append(&c, 1);
}

MyString& MyString::append(const char *chars) {
    require_non_null(chars);
    return this->append(chars, count_chars(chars));
}

MyString& MyString::append(const char *elements, int amount) {
    MyString::require_non_null(elements);
    MyString::require_valid_amount(amount);

    char *temp = nullptr;
    if (elements == this->container) {
        temp = new char[this->m_capacity + 1];
        copy_array(elements, amount, temp, this->m_capacity);
        elements = temp;
    }

    if (this->need_extend(amount)) {
        this->extend(this->m_size + amount);
    }

    for (char *cursor = this->container + this->m_size; cursor < this->container + this->m_size + amount; ++cursor) {
        *cursor = *(elements++);
    }

    delete[] temp;

    this->m_size += amount;
    this->fill_tail();
    return *this;
}

MyString& MyString::append(const MyString &str2nd) {
    return this->append(str2nd.container, str2nd.m_size);
}

MyString& MyString::insert(int position, const char &element) {
    return this->insert(position, &element, 1);
}

MyString& MyString::insert(int position, const char *chars) {
    require_non_null(chars);
    return this->insert(position, chars, count_chars(chars));
}

MyString& MyString::insert(int position, const char *elements, int amount) {
    if (position < 0) {
        position = 0;
    }
    if (position > this->m_size) {
        position = this->m_size;
    }

    MyString::require_non_null(elements);
    MyString::require_valid_amount(amount);

    char *temp = nullptr;
    if (elements == this->container) {
        temp = new char[this->m_capacity + 1];
        copy_array(elements, amount, temp, this->m_capacity);
        elements = temp;
    }

    if (this->need_extend(amount)) {
        this->extend(this->m_size + amount);
    }

    for (int cursor = this->m_size - 1; cursor >= position; --cursor) {
        this->container[cursor + amount] = this->container[cursor];
    }

    for (char *pointer = this->container + position; pointer < this->container + position + amount; ++pointer) {
        *pointer = *(elements++);
    }

    delete[] temp;

    this->m_size += amount;
    this->fill_tail();
    return *this;
}

MyString& MyString::insert(int pos, const MyString &subStr) {
    return this->insert(pos, subStr.container, subStr.m_size);
}

MyString& MyString::replace(int pos, int len, const MyString &subStr) {
    if (&subStr == this) {
        return this->replace(pos, len, MyString(subStr));
    }

    if (pos >= this->m_size) {
        pos = this->m_size - 1;
    }
    if (pos < 0) {
        pos = 0;
    }

    this->erase(pos, len);
    this->insert(pos, subStr);
    return *this;
}

MyString& MyString::replace(int pos, int len, const char *subStr) {
    return this->replace(pos, len, MyString(subStr));
}

MyString& MyString::erase(int start, int amount) {
    if (start < 0) {
        start = 0;
    }

    if (start > this->m_size) {
        start = this->m_size - 1;
    }

    if (amount < 0 || amount > this->m_size - start) {
        amount = this->m_size - start;
    }

    for (int index = start; index < this->m_size - amount; ++index) {
        this->container[index] = this->container[index + amount];
    }

    for (int index = this->m_size - amount; index < this->m_size; ++index) {
        this->container[index] = char();
    }

    this->m_size -= amount;
    this->fill_tail();

    if (this->need_shrink()) {
        this->shrink();
    }

    return *this;
}

// 0 1 2 3 4 5
// 2, 3

void MyString::clear() {
    this->m_capacity = DEFAULT_CAPACITY;
    this->m_size = 0;
    delete[] this->container; // 굳이 delete를 할까 싶긴한데... 일단은 초기 상태의 컨테이너를 가지는게 맞다고 생각하기에...
    this->container = MyString::create_empty_container(DEFAULT_CAPACITY);
}

int MyString::compare(const MyString &other) const {
    return this->compare(other.container);
}

int MyString::compare(const char *chars) const {
    const int other_length = count_chars(chars);

    for (int index = 0; index < this->m_size && index < other_length; ++index) {
        const unsigned char this_char = this->container[index];
        const unsigned char other_char = chars[index];
        if (this_char != other_char) {
            return this_char - other_char;
        }
    }

    return this->m_size - other_length;
}

bool MyString::equals(const MyString &other) const {
    if (this->m_size != other.m_size) {
        return false;
    }

    for (int index = 0; index < this->m_size; ++index) {
        if (this->container[index] != other.container[index]) {
            return false;
        }
    }

    return true;
}

MyString& MyString::operator=(const MyString &other) {
    if (this == &other) {
        return *this;
    }

    MyString temp = MyString(other);
    swap(*this, temp);
    return *this;
}

MyString& MyString::operator=(const char *chars) {
    require_non_null(chars);
    swap(*this, MyString(chars, count_chars(chars)));
    return *this;
}

MyString MyString::operator+(const MyString &str2nd) const {
    return MyString(*this).append(str2nd);
}

MyString MyString::operator+(const char *chars) const {
    require_non_null(chars);
    MyString temp = MyString(*this);
    temp.append(chars);
    return temp;
}

MyString& MyString::operator+=(const MyString &str2nd) {
    return this->append(str2nd);
}

MyString& MyString::operator+=(const char *chars) {
    require_non_null(chars);
    swap(*this, *this + chars);
    return *this;
}

void MyString::require_valid_index(int index) const {
    if (!(0 <= index && index < this->m_size)) {
        throw std::out_of_range("Sequence: index is out of range");
    }
}

void MyString::require_non_null(const char *nullable) {
    if (nullable == nullptr) {
        throw std::invalid_argument("Sequence: elements must not be null");
    }
}

void MyString::require_valid_amount(int amount) {
    if (amount < 0) {
        throw std::invalid_argument("Sequence: amount must not be negative");
    }
}

void MyString::shrink() {
    int new_capacity = MyString::calc_new_capacity(this->m_capacity / GROWTH_FACTOR);
    char *new_container = MyString::create_empty_container(new_capacity);

    copy_array(
        this->container, this->m_size,
        new_container, new_capacity
    );
    delete[] this->container;
    this->container = new_container;
    this->m_capacity = new_capacity;
}

void MyString::extend(int required) {
    int grown = this->m_capacity * GROWTH_FACTOR;
    int new_capacity = MyString::calc_new_capacity(grown > required ? grown : required);
    char *new_container = MyString::create_empty_container(new_capacity);

    copy_array(
        this->container, this->m_capacity,
        new_container, new_capacity
    );
    delete[] this->container;
    this->container = new_container;
    this->m_capacity = new_capacity;
}

void MyString::fill_tail() {
    for (int index = 0; index < TAIL; ++index) {
        this->container[this->m_size + index] = char();
    }
}

int MyString::count_chars(const char *chars) {
    require_non_null(chars);
    int count = 0;
    for (const char *cursor = chars; *cursor != '\0'; ++cursor) ++count;
    return count;
}

int MyString::rabin_karp(const MyString &pattern, int start) const {
    if (start >= this->m_size) {
        start = this->m_size - 1;
    }
    start = start < 0 ? 0 : start;

    if (pattern.m_size == 0) {
        return start;
    }

    if (pattern.m_size + start > this->m_size) {
        return -1;
    }

    // 최고차항에 곱해질 수
    long long int highest_power = 1;
    for (int loop = 1; loop < pattern.m_size; ++loop) {
        highest_power *= HASH_BASE;
        highest_power %= HASH_MODULAR;
    }

    // 첫번째 윈도우의 해싱
    long long int hashed_pattern = 0, hashed_target = 0;
    for (int index = 0; index < pattern.m_size; ++index) {
        hashed_pattern = (hashed_pattern * HASH_BASE + static_cast<unsigned char>(pattern.container[index])) %
                         HASH_MODULAR;
        hashed_target = (hashed_target * HASH_BASE + static_cast<unsigned char>(this->container[start + index])) %
                        HASH_MODULAR;
    }

    // 슬라이딩 윈도우 시작
    const int end = this->m_size - pattern.m_size;
    for (int index = start; index <= end; ++index) {
        if (index != start) {
            hashed_target -= static_cast<unsigned char>(this->container[index - 1]) * highest_power % HASH_MODULAR;

            if (hashed_target < 0) {
                hashed_target += HASH_MODULAR;
            }

            hashed_target *= HASH_BASE;


            hashed_target += static_cast<unsigned char>(this->container[index + pattern.m_size - 1]);
            hashed_target %= HASH_MODULAR;
        }

        if (hashed_pattern == hashed_target && is_same(*this, pattern, index)) {
            return index;
        }
    }

    return -1;
}

bool MyString::is_same(const MyString &target, const MyString &pattern, int pattern_start) {
    for (int index = 0; index < pattern.m_size; ++index) {
        if (target.container[index + pattern_start] != pattern.container[index]) {
            return false;
        }
    }

    return true;
}

std::ostream& operator<<(std::ostream &output_stream, const MyString &char_sequence) {
    for (int index = 0; index < char_sequence.size(); ++index) {
        output_stream << char_sequence.at(index);
    }
    return output_stream;
}

std::istream& operator>>(std::istream &input_stream, MyString &string) {
    MyString temp;
    int char_code = input_stream.get();

    // EOF를 검증을 해줘야할까..?

    // early return
    if (char_code == EOF) {
        return input_stream;
    }

    // strip lead
    while (std::isspace(char_code)) {
        char_code = input_stream.get();
    }

    for (;
        char_code != EOF;
        char_code = input_stream.get()
    ) {
        if (std::isspace(char_code)) {
            input_stream.putback(static_cast<char>(char_code));
            break;
        }

        temp.append(static_cast<char>(char_code));
    }

    string = temp;
    return input_stream;
}

std::istream& getline(std::istream &input_stream, MyString &string, char delim) {
    MyString temp;
    int char_code = input_stream.get();

    // early return
    if (char_code == EOF) {
        return input_stream;
    }

    for (;
        char_code != EOF && char_code != delim;
        char_code = input_stream.get()
    ) {
        temp.append(static_cast<char>(char_code));
    }

    string = temp;
    return input_stream;
}

int stoi(const MyString &str, int pos, int base) {
    int result = 0;
    for (int index = pos; index < str.length(); ++index) {
        int current_char_code = static_cast<unsigned char>(str[index]);
        int digit;


        if ('0' <= current_char_code && current_char_code <= '9') {
            digit = current_char_code - '0';
        } else if ('a' <= current_char_code && current_char_code <= 'z') {
            digit = current_char_code - 'a' + 10;
        } else if ('A' <= current_char_code && current_char_code <= 'Z') {
            digit = current_char_code - 'A' + 10;
        } else {
            break;
        }

        if (digit >= base) {
            break;
        }

        result = result * base + digit;
    }

    return result;
}
