#include "Worker.h"

Worker::Worker(string f, string s, string t, int n) : TPerson(f, s, t)
{
    SetTNumber(n);
}
void Worker::SetTNumber(int number)
{
    tabelNumber = number;
}
string Worker::FullData()
{
    return TPerson::FullData() + " " + to_string(tabelNumber);
}
string Worker::GetType() const { return "W"; }
