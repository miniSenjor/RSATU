#pragma once
#include "TPerson.h"
class Worker : public TPerson
{
protected:
    int tabelNumber;
public:
    Worker(string f, string s, string t, int n);
    void SetTNumber(int number);
    string FullData() override;
    string GetType() const override;
};

