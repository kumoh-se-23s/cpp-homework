#include <iostream>
#include <optional>
using namespace std;

class MySet {
    public: 
    private:
        int* arr;
        unsigned int size;
        unsigned int capacity;

    public:
        MySet();
        MySet(const MySet& mySet);

        optional<int> findInsertIndex(int inputNum);
        void insertElement(int insertIndex, int insertNum);

        void resize();
        int length() const;
        int getElement(int index) const;
        void appendElement(int element);

        MySet operator+(const MySet inputMySet);
        MySet operator&(const MySet inputMySet);
        MySet operator-(const MySet inputMySet);
        void operator=(const MySet mySet);

};

istream& operator>>(istream& inputStream , MySet& mySet);
ostream& operator<<(ostream& outputStream , MySet& mySet);