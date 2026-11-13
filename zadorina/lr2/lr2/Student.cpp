#include "Student.h"

Student::Student(string f, string s, string t, int course, string group) : TPerson(f, s, t)
{
    SetGroup(group);
    SetCourse(course);
}
void Student::SetGroup(string g)
{
    group = g;
}
void Student::SetCourse(int c)
{
    if (c > 0 && c < 8)
        course = c;
    else
        throw invalid_argument("не корректное значение курса");
}
string Student::FullData()
{
    return TPerson::FullData() + " " + to_string(course) + " " + group;
}
string Student::GetType() const { return "S"; }
