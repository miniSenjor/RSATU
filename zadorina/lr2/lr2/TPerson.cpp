#include "TPerson.h"
#include <string>
#include <fstream>
using namespace std;
string TPerson::ToUpper(string s)
{
    for (char& c : s) {
        c = toupper(c);
    }
    return s;
}

string TPerson::ToLower(string s)
{
    for (char& c : s) {
        c = tolower(c);
    }
    return s;
}

TPerson::TPerson(ifstream& file)
{
    string f, s, t;
    file >> f >> s >> t;
    SetFName(f);
    SetSName(s);
    SetTName(t);
}
TPerson::TPerson(string f, string s, string t)
{
    SetFName(f);
    SetSName(s);
    SetTName(t);
}

void TPerson::SetFName(string f)
{
    fName = ToUpper(f);
}

void TPerson::SetSName(string s)
{
    sName = ToLower(s);
}

void TPerson::SetTName(string t)
{
    tName = ToLower(t);
}

string TPerson::FullData()
{
    return fName + " " + sName + " " + tName;
}

string TPerson::GetType() const { return "P"; }

void TPerson::WriteData(ofstream& file)
{
    if (file.is_open())
        file << FullData();
}
