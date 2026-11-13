#pragma once
class Matrix
{
private:
    float** mas;
    int n, m;
public:
    Matrix(int a, int b);
    ~Matrix();

    void operator=(Matrix& m2);

    friend bool operator==(Matrix& l, Matrix& r);

    friend Matrix operator+(const Matrix& l, const Matrix& r);

    friend Matrix operator+(const Matrix& l, int x);

    friend Matrix operator-(const Matrix& l, const Matrix& r);

    friend Matrix operator*(const Matrix& l, const Matrix& r);

    friend Matrix operator*(const Matrix& l, int x);
    
    friend Matrix operator++(Matrix& l);

    void ReadMas();
    void WriteMas();
};