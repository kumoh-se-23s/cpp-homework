#include "Matrix.h"

#include <iostream>
using namespace std;

void Matrix::setValue(int rowIdx, int colIdx, int value)
{
    matrix[rowIdx][colIdx] = value;
}
int Matrix::getValue(int rowIdx, int colIdx)
{
    return matrix[rowIdx][colIdx];
}
void Matrix::read()
{
    for (int rowIdx = 0; rowIdx < MAX_SIZE; rowIdx++)
    {
        for (int colIdx = 0; colIdx < MAX_SIZE; colIdx++)
        {
            int inputValue;
            cin >> inputValue;
            setValue(rowIdx, colIdx, inputValue);
        }
    }
}
void Matrix::print()
{
    for (int rowIdx = 0; rowIdx < MAX_SIZE; rowIdx++)
    {
        cout << "|";
        for (int colIdx = 0; colIdx < MAX_SIZE; colIdx++)
            printf("%4d", matrix[rowIdx][colIdx]);
        cout << "|";
        cout << endl;
    }
}
Matrix Matrix::transpose()
{
    Matrix tempMatrix;
    for (int rowIdx = 0; rowIdx < MAX_SIZE; rowIdx++)
    {
        for (int colIdx = 0; colIdx < MAX_SIZE; colIdx++)
            tempMatrix.setValue(colIdx, rowIdx, matrix[rowIdx][colIdx]);
    }
    return tempMatrix;
}
Matrix Matrix::add(Matrix otherMatrix)
{
    Matrix tempMatrix;
    for (int rowIdx = 0; rowIdx < MAX_SIZE; rowIdx++)
    {
        for (int colIdx = 0; colIdx < MAX_SIZE; colIdx++)
        {
            tempMatrix.setValue(
                rowIdx, colIdx,
                this->matrix[rowIdx][colIdx] + otherMatrix.getValue(rowIdx, colIdx));
        }
    }
    return tempMatrix;
}
Matrix Matrix::multi(Matrix otherMatrix)
{
    Matrix tempMatrix;
    for (int rowIdx = 0; rowIdx < MAX_SIZE; rowIdx++)
        for (int colIdx = 0; colIdx < MAX_SIZE; colIdx++)
            tempMatrix.setValue(rowIdx, colIdx, 0);

    for (int i = 0; i < MAX_SIZE; i++)
        for (int j = 0; j < MAX_SIZE; j++)
            for (int k = 0; k < MAX_SIZE; k++)
                tempMatrix.setValue(
                    i, j,
                    this->matrix[i][k] * otherMatrix.getValue(k, j) + tempMatrix.getValue(i, j));
    return tempMatrix;
}