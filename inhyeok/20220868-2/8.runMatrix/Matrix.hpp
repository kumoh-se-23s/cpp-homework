

#pragma once
#include <iomanip>
#include <iostream>

template <int SIZE>
class Matrix
{

    int data[SIZE][SIZE];

    public:

    void read(){
        using namespace std;
        for(int i = 0; i < SIZE; ++i){
            for(int j = 0; j < SIZE; ++j){
                cin >> data[i][j];
            }   
        }
    }

    Matrix transpose() const
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
    Matrix add(const Matrix &other) const
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
    Matrix multi(const Matrix &other) const
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
    void print() const
    {
        using namespace std;
        for (int i = 0; i < SIZE; ++i)
        {
            cout << "|";
            for (int j = 0; j < SIZE; ++j)
            {
                cout << setw(4) << data[i][j];
            }
            cout << " |" << endl;
        }
    }
};