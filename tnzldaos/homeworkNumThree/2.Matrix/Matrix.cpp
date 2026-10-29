#include "Matrix.h"
#include <iostream>
#include <cmath>
using namespace std;

    int calculateLength(const Matrix& m1) {
        int longestInputLength = 1;

        for (int column = 0 ; column < m1.SIZE ; column++) {
            for(int row = 0 ; row < m1.SIZE ; row++) {    
                int element = m1.getElement(column,row) > 0 ? m1.getElement(column,row) : -(m1.getElement(column,row)); 

                while(element / pow(10, longestInputLength) >= 1) {
                    longestInputLength = longestInputLength + 1;
                }
            }
        }
        return longestInputLength;
    }



    Matrix::Matrix() {
    }

    int Matrix::getLongestInputLength() const {
        return longestInputLength;
    }

    void Matrix::setLongestInputLength(int inputLength) {
        longestInputLength = inputLength;
    }

    int Matrix::getElement(int column, int row) const {
        return matrix[column][row];
    }

    void Matrix::setElement(int column, int row, int value) {  
        matrix[column][row] = value;
    }

    const Matrix Matrix::operator!() {
        Matrix resultMatrix = Matrix();

        for (int column = 0 ; column < SIZE ; column++) {
            for(int row = 0 ; row < SIZE ; row++) {
                resultMatrix.matrix[row][column] = matrix[column][row];
            }
        }

        return resultMatrix;
    }

    const Matrix Matrix::operator+(const Matrix& m1) {
        Matrix resultMatrix = Matrix();

        for (int column = 0 ; column < SIZE ; column++) {
            for(int row = 0 ; row < SIZE ; row++) {    
                resultMatrix.matrix[column][row] =  matrix[column][row] + m1.getElement(column, row);
            }
        }

        return resultMatrix;
    }

    const Matrix Matrix::operator*(const Matrix& m1) {
        Matrix resultMatrix = Matrix();
        int resultRowPosition = 0;

        for(int column = 0 ; column < SIZE * SIZE ; column++) {
            int result = 0;

            for(int row = 0 ; row < SIZE ; row++) {
            
                result = result + matrix[column / SIZE][row] * m1.getElement(row, column % SIZE);
            }

            resultMatrix.matrix[column / SIZE][resultRowPosition % SIZE] = result;
            resultRowPosition++;
        }

        return resultMatrix;
    }

    ostream& operator<<(ostream& outputStream, const Matrix& m1) { 
        int width = calculateLength(m1);

        for (int column = 0 ; column < m1.SIZE ; column++) {
            outputStream << "|";

            for(int row = 0 ; row < m1.SIZE ; row++) {
                printf("%*d", width + 2 ,m1.getElement(column,row));
            }
        
            outputStream << " | \n";
        }

        return outputStream;
    }

    istream& operator>>(istream& inputStream, Matrix& m1) {
        for(int column = 0 ; column < m1.SIZE ; column++) {
            for(int row = 0 ; row < m1.SIZE ; row++) {
                int value{0};
                inputStream >> value;

                m1.setElement(column, row, value);
            }
        }

        return inputStream;
    }