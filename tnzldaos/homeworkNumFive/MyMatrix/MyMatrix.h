#include <iostream>
using namespace std;

class MyMatrix {
    public: 
    private:
        int column;
        int row;
        int** arr;

    public:
        MyMatrix();
        ~MyMatrix();
        MyMatrix(const MyMatrix& myMatrix);
        MyMatrix(int column, int row);

        void init(const int* arr);

        int getColumn() const;
        void setColumn(int column);
        int getRow() const;
        void setRow(int row);

        int getElement(int column, int row) const;

        MyMatrix operator+(const MyMatrix& inputMyMatrix) const;
        MyMatrix operator*(const MyMatrix& inputMyMatrix) const;
        MyMatrix& operator=(const MyMatrix& inputMyMatrix);
};

ostream& operator<<(ostream& outputStream, const MyMatrix& myMatrix);