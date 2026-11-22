#pragma once
#include <iostream>
#include <cstddef>

using namespace std;

template <class T>
class Array {
private:
    int n;
    T* array;
public:
    Array(int N) {
        n = N;
        array = new T[n];
    }
    template <size_t N>
    Array(T(&ar)[N]) : n(N), array(new T[N]) {
        for (int i = 0; i < n; ++i)
            array[i] = ar[i];
    }
    Array(T*&ar, int N) : n(N), array(new T[N]) {
        for (int i = 0; i < n; ++i)
            array[i] = ar[i];
    }
    Array(const Array& other) : n(other.n), array(new T[n]) {
        for (int i = 0; i < n; ++i)
            array[i] = other.array[i];
    }
    ~Array() {
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
    int size() {
        return n;
    }

    void Print() {
        for (int i = 0; i < n; i++)
            std::cout << array[i] << " ";
        std::cout << std::endl;
    }
};