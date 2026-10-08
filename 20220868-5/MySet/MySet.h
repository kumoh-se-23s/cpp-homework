

#pragma once
#include <iostream>
#include "MyArray.hpp"

class MySet{
    MyArray<> elements;
    
public:
    explicit MySet(int capacity = 4);
    
    ~MySet();
    MySet(const MySet& other) noexcept;
    MySet operator=(const MySet& other) noexcept;

    
    MySet(MySet&& other) noexcept;
    MySet operator=(MySet&& other) noexcept;
    MySet operator&(const MySet &other) const;
    MySet operator+(const MySet &other);
    MySet operator-(const MySet &other);
    int findPlacementIndex(int value) const;
    void add(int value);
    void remove(int value);
    bool contains(int value) const;
    const MyArray<int> &getData() const;
    void clear();

    MyArray<int> &getData();

};


std::ostream &operator<<(std::ostream &out, MySet &obj){
    
    for(int i = 0; i < obj.getData().getSize(); ++i){
        out << obj.getData()[i] << " ";
    }

    return out;
}


std::istream &operator>>(std::istream &in, MySet &obj){
    
    int input = 0;

    while(input >= 0){
        in >> input;
        if (input >= 0) obj.getData().add(input);
    }
    obj.getData().mergeSort();
    return in;
}