
#pragma once
#include <cstdint>


template <int SIZE>
class MyArray
{
    int arr[SIZE];

public:
    MyArray() : arr(0)
    {
    }

    int &operator[](int idx)
    {
        return arr[idx];
    }

    const int &operator[](int idx) const
    {
        return arr[idx];
    }

    bool readAscSorted()
    {
        using namespace std;
        int previousInput = 0;

        for (int i = 0; i < SIZE; ++i)
        {
            cin >> arr[i];
            if (arr[i] < previousInput)
            {
                return false;
            }
            previousInput = arr[i];
        }
        return true;
    }

    void print() const
    {
        using namespace std;

        for (int i = 0; i < SIZE; ++i)
        {
            cout << (*this)[i] << " ";
        }
        cout << endl;
    }

    MyArray<SIZE * 2> merge(const MyArray<SIZE> &other) const
    {
        using namespace std;
        int aLoc = 0;
        int bLoc = 0;

        MyArray<SIZE * 2> result;

        for (int loc = 0; loc < SIZE * 2; ++loc)
        {

            if (bLoc == SIZE || (aLoc < SIZE && (*this)[aLoc] < other[bLoc]))
            {
                result[loc] = (*this)[aLoc];
                ++aLoc;
            }
            else
            {
                result[loc] = other[bLoc];
                ++bLoc;
            }
        }
        return result;
    }
};