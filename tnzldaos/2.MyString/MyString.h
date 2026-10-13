#include "CharSequence.h"

class MyString : public CharSequence {
    public:
    private:

    public:
        MyString();
        MyString(char inputCharArr[]);
        MyString(const MyString &srcStr);

        int length() const override;
        bool empty() const override;
        char at(int pos) const override;

        int find(const char subStr[], int pos=0) const override;
        int find(const MyString &subStr, int pos=0) const;

        MyString substr(int pos, int len);

        const MyString operator+(const char str2nd[]);
        const MyString operator+(const MyString& str2nd);
        void operator=(const MyString &srcStr);
        MyString& operator=(const char str2nd[]);
        char operator[](int pos);

        bool operator==(const MyString &str);
        bool operator!=(const MyString &str);

        // this->CharSequence::find()
    private:

};