#include "Matrix.h"
#include <stdexcept>
#include <iostream>


Matrix::Matrix(int a, int b) {
    n = a;m = b;
    mas = new float* [n];
    for (int i = 0;i < n;i++)
        mas[i] = new float[m];
}
Matrix::~Matrix()
{
    for (int i = 0;i < n;i++)
        delete[] mas[i];
    delete[]mas;
}

void Matrix::operator=(Matrix& m2)
{
    if (n == m2.n && m == m2.m)
        for (int i = 0;i < n;i++)
            for (int j = 0;j < m;j++)
                mas[i][j] = m2.mas[i][j];
    else throw std::invalid_argument("Не совпадают размеры массивов");

}

bool operator==(Matrix& l, Matrix& r)
{
    if (l.n != r.n || l.m != r.m)
        throw std::invalid_argument("Не совпадают размеры массивов");

    for (int i = 0;i < l.n;i++)
        for (int j = 0;j < l.m;j++)
            if (l.mas[i][j] != r.mas[i][j])
                return false;
    return true;
}

Matrix operator+(const Matrix& l, const Matrix& r) {
    if (l.n != r.n || l.m != r.m)
        throw std::invalid_argument("Не совпадают размеры массивов");
    Matrix m(l.n, l.m);
    for (int i = 0; i < m.n; i++)
        for (int j = 0; j < m.m; j++)
            m.mas[i][j] = l.mas[i][j] + r.mas[i][j];
    return m;
}

Matrix operator+(const Matrix& l, int x) {
    Matrix m(l.n, l.m);
    for (int i = 0; i < m.n; i++)
        for (int j = 0; j < m.m; j++)
            m.mas[i][j] = l.mas[i][j] + x;
    return m;
}

Matrix operator-(const Matrix& l, const Matrix& r) {
    if (l.n != r.n || l.m != r.m)
        throw std::invalid_argument("Не совпадают размеры массивов");
    Matrix m(l.n, l.m);
    for (int i = 0; i < m.n; i++)
        for (int j = 0; j < m.m; j++)
            m.mas[i][j] = l.mas[i][j] - r.mas[i][j];
    return m;
}

Matrix operator*(const Matrix& l, const Matrix& r)
{
    if (l.m != r.n)
        throw std::invalid_argument("Не совпадают размеры массивов");

    Matrix tmp(l.n, r.m);
    float s;
    for (int i = 0;i < l.n;i++)
        for (int j = 0;j < r.m;j++)
        {
            s = 0;
            for (int k = 0;k < l.m;k++)
                s += l.mas[i][k] * r.mas[k][j];
            tmp.mas[i][j] = s;
        }
    return tmp;
}

Matrix operator*(const Matrix& l, int x)
{
    Matrix tmp(l.n, l.m);
    int s;
    for (int i = 0;i < l.n;i++)
        for (int j = 0;j < l.m;j++)
            tmp.mas[i][j] = l.mas[i][j] * x;
    return tmp;
}

Matrix operator++(Matrix& l)
{
    Matrix tmp(l.m, l.n);
    for (int i = 0; i < l.n; i++)
        for (int j = 0; j < l.m; j++)
            tmp.mas[j][i] = l.mas[i][j];
    
    return tmp;
}

void Matrix::ReadMas()
{
    std::cout << "Введите массив" << std::endl;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            std::cin >> mas[i][j];
}

void Matrix::WriteMas()
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
            std::cout << mas[i][j] << " ";
        std::cout << std::endl;
    }
}