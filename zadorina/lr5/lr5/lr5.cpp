#include <iostream>
#include <windows.h>

using namespace std;

struct complex {
    double re, im;
    friend complex operator*(const complex& x, const complex& y){
        complex c;
        c.re = x.re * y.re - x.im * y.im;
        c.im = x.re * y.im + x.im * y.re;
        return c;
    }
};

template <class T> 
class Array {
private:
    int n;
    T* array;
public:
    Array(int N){
        n = N;
        array = new T[n];
    }
    template <size_t N>
    Array(T (&ar)[N]) : n(N), array(new T[N]) {
        for (int i = 0; i < n; ++i)
            array[i] = ar[i];
    }
    Array(const Array& other) : n(other.n), array(new T[n]) {
        for (int i = 0; i < n; ++i)
            array[i] = other.array[i];
    }
    ~Array(){
        delete[]array;
    }

    Array& operator=(const Array& other) {
        if (this != &other) {
            delete[] array;
            n = other.n;
            array = new T[n];
            for (int i = 0; i < n; ++i)
                array[i] = other.array[i];
        }
        return *this;
    }

    friend Array operator*(const Array& l, const Array& r) {
        if (l.n != r.n)
            throw "У массивов разная длина";
        Array tmp(l.n);
        for (int i = 0; i < l.n; i++)
            tmp.array[i] = l.array[i] * r.array[i];
        return tmp;
    }
    friend bool operator==(const Array& l, const Array& r) {
        if (l.n != r.n)
            throw "У массивов разная длина";
        for (int i = 0; i < l.n; i++)
            if (l.array[i] != r.array[i])
                return false;
        return true;
    }
    int size(){
        return n;
    }

    void Print(){
        for (int i = 0; i < n; i++)
            cout << array[i] << " ";
        cout << endl;
    }
};

int main() {
    SetConsoleOutputCP(1251);
    int intNums1[] = { 1, 2, 3, 4, 5 };
    int intNums2[] = { 5, 4, 3, 2, 1 };
    float fNums1[] = { 1.2f, 3.5f, 7.9f, 9.0f, 11.1f };
    float fNums2[] = { 2.0f, 0.1f, 5.0f, 6.1f, 8.4f };
    Array<int> intArray1(intNums1);
    Array<int> intArray2(intNums2);
    Array<float> fArray1(fNums1);
    Array<float> fArray2(fNums2);

    if (fArray1 == fArray2)
        cout << "Массивы равны" << endl;
    else
        cout << "Массивы не равны" << endl;

    Array<int> res = intArray1 * intArray2;
    res.Print();

    return 0;
}