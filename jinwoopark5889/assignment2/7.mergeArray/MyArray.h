#pragma once

class MyArray2
{
public:
    void print() const;
    int getArrValue(int);
    void setArrValue(int, int);

private:
    static const int MAX_SIZE = 10;
    int arr[MAX_SIZE];
};
class MyArray1
{
public:
    bool readAscSorted();
    MyArray2 merge(MyArray1);
    int getArrValue(int);

private:
    static const int MAX_SIZE = 5;
    int arr[MAX_SIZE];
};