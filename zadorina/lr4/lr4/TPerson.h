#pragma once
#include <string>
#include <fstream>
using namespace std;
class TPerson
{
private:
    string ToUpper(string s);

    string ToLower(string s);
protected:
    string fName, sName, tName;

public:
    TPerson(ifstream& file);
    TPerson(string f, string s, string t);

    void SetFName(string f);

    void SetSName(string s);

    void SetTName(string t);

    virtual string FullData();

    virtual string GetType() const;

    void WriteData(ofstream& file);
};

