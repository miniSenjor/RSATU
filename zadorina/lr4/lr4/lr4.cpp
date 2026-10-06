#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

class TPerson
{
private:
    string ToUpper(string s)
    {
        for (char c : s) {
            c = toupper(c);
        }
        return s;
    }

    string ToLower(string s)
    {
        for (char c : s) {
            c = tolower(c);
        }
        return s;
    }
protected:
    string fName, sName, tName;

public:
    TPerson(ifstream& file)
    {
        string f, s, t;
        file >> f >> s >> t;
        SetFName(f);
        SetSName(s);
        SetTName(t);
    }
    TPerson(string f, string s, string t)
    {
        SetFName(f);
        SetSName(s);
        SetTName(t);
    }

    void SetFName(string f)
    {
        fName = ToUpper(f);
    }

    void SetSName(string s)
    {
        sName = ToLower(s);
    }

    void SetTName(string t)
    {
        tName = ToLower(t);
    }

    virtual string FullData()
    {
        return fName + " " + sName + " " + tName;
    }

    void WriteData(ofstream& file)
    {
        if (file.is_open())
            file << FullData();
    }
};

class Worker : public TPerson
{
protected:
    int tabelNumber;
public:
    Worker(string f, string s, string t, int n) : TPerson(f, s, t)
    {
        SetTNumber(n);
    }
    void SetTNumber(int number)
    {
        tabelNumber = number;
    }
    string FullData() override
    {
        return TPerson::FullData() + " " + to_string(tabelNumber);
    }
};

class Student : public TPerson
{
protected:
    int course;
    string group;
public:
    Student(string f, string s, string t, int course, string group) : TPerson(f, s, t)
    {
        SetGroup(group);
        SetCourse(course);
    }
    void SetGroup(string g)
    {
        group = g;
    }
    void SetCourse(int c)
    {
        course = c;
    }
    string FullData() override
    {
        return TPerson::FullData() + " " + to_string(course) + " " + group;
    }
};

class TSpisok
{
private:
    vector<TPerson> persons;
public:
    TSpisok()
    {

    }
};

int main()
{
    string s = "fFFf";
    TPerson p(s, s, s);
    Student st(s, s, s, 1, s);
    Worker w(s, s, s, 1);

}
