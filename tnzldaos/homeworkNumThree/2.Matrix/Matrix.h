#include <iostream>
using namespace std;

class Matrix{
    public:
        const static int SIZE{3};
        int longestInputLength{1};

        Matrix();
    
    private:
        int matrix [SIZE][SIZE];

    public:
        int getLongestInputLength() const;
        void setLongestInputLength(int inputLength);

        int getElement(int column, int row) const;
        void setElement(int column, int row, int value);

        const Matrix operator!();
        const Matrix operator+(const Matrix& m1);
        const Matrix operator*(const Matrix& m1);

};

ostream& operator<<(ostream& outputStream, const Matrix& m1);

istream& operator>>(istream& inputStream, Matrix& m1);