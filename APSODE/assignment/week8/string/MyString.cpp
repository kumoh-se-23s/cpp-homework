//
// Created by apsode on 26. 11. 2..
//

#include "MyString.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdio>
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

const char& MyString::at(int index) const {
    return (*this)[index];
}

char& MyString::at(int index) {
    return (*this)[index];
}

const char& MyString::operator[](int index) const {
    if (!is_valid_index(index)) {
        return this->container[this->m_size];
    }
    return this->container[index];
}

char& MyString::operator[](int index) {
    if (!is_valid_index(index)) {
        return this->container[this->m_size];
    }
    return this->container[index];
}

int MyString::find(const MyString &subStr, int position) const {
    return this->rabin_karp(subStr, position);
}

int MyString::find(const char *pattern, int start) const {
    return this->rabin_karp(MyString(pattern, count_chars(pattern)), start);
}

MyString MyString::substr(int position, int length) const {
    MyString result = MyString();

    if (position >= this->m_size || length <= 0) {
        return result;
    }

    if (position < 0) {
        // +00 +01 +02 +03 +04 +05 +06 +07 +08 +09
        // -10 -09 -08 -07 -06 -05 -04 -03 -02 -01
        // p:-3, l:3, cl:10 -> -3 + 10
        int temp = position + this->m_size + 1;
        length = std::min(length, temp);
        position = temp - length;
    }

    for (int index = 0; index < length && position + index < this->m_size; ++index) {
        result.append(this->container[position + index]);
    }

    return result;
}

MyString& MyString::append(char character) {
    return this->append(&character, 1);
}

MyString& MyString::append(const char *chars) {
    require_non_null(chars);
    return this->append(chars, count_chars(chars));
}

MyString& MyString::append(const char *elements, int amount) {
    MyString::require_non_null(elements);
    MyString::require_valid_amount(amount);

    char *temp = nullptr;
    if (this->is_alias_container(elements)) {
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
    if (position > this->m_size) {
        position = this->m_size - 1;
    }

    if (position < 0) {
        position = 0;
    }

    MyString::require_non_null(elements);
    MyString::require_valid_amount(amount);

    char *temp = nullptr;
    if (this->is_alias_container(elements)) {
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

MyString& MyString::insert(int position, const MyString &subStr) {
    return this->insert(position, subStr.container, subStr.m_size);
}

MyString& MyString::replace(int position, int length, const MyString &subStr) {
    if (&subStr == this) {
        return this->replace(position, length, MyString(subStr));
    }

    if (position > this->m_size) {
        position = this->m_size - 1;
    }

    if (position < 0) {
        position = 0;
    }

    this->erase(position, length);
    this->insert(position, subStr);
    return *this;
}

MyString& MyString::replace(int position, int length, const char *subStr) {
    return this->replace(position, length, MyString(subStr));
}

MyString& MyString::erase(int start, int amount) {
    if (start > this->m_size) {
        start = this->m_size - 1;
    }

    if (start < 0) {
        start = 0;
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
    delete[] this->container; // 굳이 delete&create를 할까 싶긴한데... 일단은 초기 상태의 컨테이너를 가지는게 맞다고 생각하기에...
    this->container = MyString::create_empty_container(DEFAULT_CAPACITY);
}

int MyString::compare(const MyString &other) const {
    return this->compare(other.container, other.m_size);
}

int MyString::compare(const char *chars) const {
    return this->compare(chars, count_chars(chars));
}

int MyString::compare(const char *chars, int length) const {
    require_non_null(chars);
    require_valid_amount(length);

    for (int index = 0; index < this->m_size && index < length; ++index) {
        const unsigned char this_char = this->container[index];
        const unsigned char other_char = chars[index];
        if (this_char != other_char) {
            return this_char - other_char;
        }
    }

    return this->m_size - length;
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
    return MyString(*this).append(chars);
}

MyString& MyString::operator+=(const MyString &str2nd) {
    return this->append(str2nd);
}

MyString& MyString::operator+=(const char *chars) {
    require_non_null(chars);
    return this->append(chars);
}

bool MyString::is_valid_index(int index) const {
    return 0 <= index && index < this->m_size;
}

bool MyString::is_alias_container(const char *maybe_this) const {
    return this->container <= maybe_this && maybe_this < this->container + this->m_capacity;
}

void MyString::require_non_null(const char *nullable) {
    if (nullable == nullptr) {
        throw std::invalid_argument("MyString: elements must not be null");
    }
}

void MyString::require_valid_amount(int amount) {
    if (amount < 0) {
        throw std::invalid_argument("MyString: amount must not be negative");
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
    int new_capacity = this->m_capacity;
    while (new_capacity < required) {
        new_capacity *= GROWTH_FACTOR;
    }
    char* new_container = MyString::create_empty_container(new_capacity);

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
    // start 범위 밖 검증
    if (start > this->m_size) {
        start = this->m_size - 1;
    }

    // start 음수 검증
    if (start < 0) {
        start = 0;
    }

    // 빈 패턴 검증
    if (pattern.m_size == 0) {
        return start;
    }

    // 시작~끝 보다 패턴이 긴 경우 fail
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
        hashed_pattern = (hashed_pattern * HASH_BASE + static_cast<unsigned char>(pattern.container[index])) % HASH_MODULAR;
        hashed_target = (hashed_target * HASH_BASE + static_cast<unsigned char>(this->container[start + index])) % HASH_MODULAR;
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

std::ostream& operator<<(std::ostream &output_stream, const MyString &string) {
    for (int index = 0; index < string.length(); ++index) {
        output_stream << string.at(index);
    }
    return output_stream;
}

std::istream& operator>>(std::istream &input_stream, MyString &string) {
    MyString temp = MyString();
    int char_code = input_stream.peek();


    // EOF Check
    if (char_code == EOF) {
        input_stream.setstate(std::ios::failbit);
        return input_stream;
    }

    // strip lead (only space char)
    for (
        char_code = input_stream.peek();
        std::isspace(char_code) && char_code != EOF;
        char_code = input_stream.peek()
    ) {
        input_stream.get();
    }

    // peek loop
    for (
        char_code = input_stream.peek();
        char_code != EOF;
        char_code = input_stream.peek()
    ) {
        if (std::isspace(char_code)) {
            break;
        }

        temp.append(static_cast<char>(input_stream.get()));
    }

    if (temp.isEmpty()) {
        input_stream.setstate(std::ios::failbit);
        return input_stream;
    }

    string = temp;
    return input_stream;
}

std::istream& getline(std::istream &input_stream, MyString &string, char delim) {
    // bad(ㅠㅠ) stream check
    if (!input_stream.good()) {
        input_stream.setstate(std::ios::failbit);
        return input_stream;
    }

    MyString temp = MyString();
    bool extracted = false;
    int char_code = input_stream.peek();
    // non ascii delim guard
    unsigned char unsigned_delim = static_cast<unsigned char>(delim);

    // non extracted => Failbit
    // extracted -> Success
    for (
        char_code = input_stream.peek();
        char_code != EOF;
        char_code = input_stream.peek()
    ) {
        extracted = true;
        if (char_code == unsigned_delim) {
            input_stream.get();
            break;
        }

        temp.append(static_cast<char>(input_stream.get()));
    }

    if (!extracted) {
        input_stream.setstate(std::ios::failbit);
    }

    string = temp;
    return input_stream;
}

int stoi(const MyString &string, int position, int base) {
    if (position < 0) {
        position = 0;
    }

    if (position >= string.length()) {
        position = string.length() - 1;
    }

    int result = 0;
    for (int index = position; index < string.length(); ++index) {
        // non ascii support
        int current_char_code = static_cast<unsigned char>(string[index]);
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

int stoi(const char *chars, int position, int base) {
    return stoi(MyString(chars), position, base);
}
