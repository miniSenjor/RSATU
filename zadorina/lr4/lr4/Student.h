#pragma once
#include "TPerson.h"
class Student : public TPerson
{
protected:
    int course;
    string group;
public:
    Student(string f, string s, string t, int course, string group);
    void SetGroup(string g);
    void SetCourse(int c);
    string FullData() override;
    string GetType() const override;
};

