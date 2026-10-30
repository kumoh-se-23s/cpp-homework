

#pragma once
#include <iomanip>
#include <string>
#include <format>
#include <sstream>

class MyMatrix
{

    int **data = nullptr;
    int rows = 0;
    int cols = 0;

public:

    MyMatrix(int rows = 0, int cols = 0);

    ~MyMatrix();

    MyMatrix(const MyMatrix &other) noexcept;

    MyMatrix &operator=(const MyMatrix &other) noexcept;

    MyMatrix(MyMatrix &&other) noexcept;

    MyMatrix &operator=(MyMatrix &&other) noexcept;


    [[nodiscard]] const int * const*getData() const
    {
        return data;
    }

    int ** getData()
    {
        return data;
    }

    MyMatrix operator!() const;
    MyMatrix operator+(const MyMatrix &other) const;
    MyMatrix operator*(const MyMatrix &other) const;
    [[nodiscard]] int getCols() const { return cols; }
    [[nodiscard]] int getRows() const { return rows; }

    void init(const int * arr) const;


    static int getLen(int v);

private:
    [[nodiscard]] static MyMatrix invalidMatrixSentinel() {
        auto m = MyMatrix(1, 1);
        m.getData()[0][0] = -999;
        return m;
    }
};



std::ostream &operator<<(std::ostream &out, const MyMatrix &v)
{

    using namespace std;
    int * const maxWidths = new int[v.getCols()]{};

    for (int i = 0; i < v.getRows(); ++i)
    {
        for (int j = 0; j < v.getCols(); ++j)
        {
            maxWidths[j] = std::max(maxWidths[j], MyMatrix::getLen(v.getData()[i][j]));
        }
    }

    for (int i = 0; i < v.getRows(); ++i)
    {
        out << "|";
        for (int j = 0; j < v.getCols(); ++j)
        {
            out << setw(maxWidths[j] + 1) << v.getData()[i][j];
        }
        out << " |" << endl;
    }
    delete[] maxWidths;
    return out;
};

std::istream &operator>>(std::istream &in, MyMatrix &v)
{
    auto data = v.getData();
    for (int i = 0; i < v.getRows(); ++i)
    {
        for (int j = 0; j < v.getCols(); ++j)
        {
            in >> data[i][j];
        }
    }
    return in;
}
