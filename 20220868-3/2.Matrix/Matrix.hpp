

#pragma once
#include <iomanip>
#include <iostream>
#include <string>
#include <format>
#include <sstream>

template <int SIZE = 3>
class Matrix
{

    int data[SIZE][SIZE];

    public:
    
    const int ** getData() const {
        return data;
    }
    
    auto getData() {
        return data;
    }


    Matrix operator!() const
    {
        Matrix result;
        for (int i = 0; i < SIZE; ++i)
        {
            for (int j = 0; j < SIZE; ++j)
            {
                result.data[j][i] = data[i][j];
            }
        }
        return result;
    }
    Matrix operator+(const Matrix &other) const
    {
        Matrix result;
        for (int i = 0; i < SIZE; ++i)
        {
            for (int j = 0; j < SIZE; ++j)
            {
                result.data[i][j] = data[i][j] + other.data[i][j];
            }
        }
        return result;
    }
    Matrix operator*(const Matrix &other) const
    {
        Matrix result;
        for (int i = 0; i < SIZE; ++i)
        {
            for (int j = 0; j < SIZE; ++j)
            {
                int sum = 0;
                for (int k = 0; k < SIZE; ++k)
                {
                    sum += data[i][k] * other.data[k][j];
                }
                result.data[i][j] = sum;
            }
        }
        return result;
    }

    static int getLen(int v){
        int cnt = (v < 0);
        for(;v != 0; v /= 10){
            ++cnt;
        }  
        return cnt;
    }


    std::string toString() const
    {   
        
        using namespace std;
        int maxWidths[SIZE] = {};
        ostringstream oss;
        
        for (int i = 0; i < SIZE; ++i)
        {
            for (int j = 0; j < SIZE; ++j)
            {
                maxWidths[j] = std::max(maxWidths[j], getLen(data[i][j]));
            }
        }
        

        for (int i = 0; i < SIZE; ++i)
        {
            oss << "|";
            for (int j = 0; j < SIZE; ++j)
            {
                oss << setw(maxWidths[j] + 1) << data[i][j];
            }
            oss << " |" << endl;
        }
        std::string_view sv = oss.view();
        std::string result = sv.data();
        return result;
    }
};

template<int SIZE>
std::istream & operator>>(std::istream &in, Matrix<SIZE> &v){
    auto data = v.getData();
    for(int i = 0; i < SIZE; ++i){
        for(int j = 0; j < SIZE; ++j){
            in >> data[i][j];
        }   
    }
    return in;
};
    

template<int SIZE>
std::ostream & operator<<(std::ostream &out, const Matrix<SIZE> &v){
    return out << v.toString();
};