#include <iostream>
#include <map>
using namespace std;

class TDat
{
public:
	map<double, double> m;
	void AddPoint(double x, double fx)
	{
		m[x] = fx;
	}
};

class TDatLinInt : TDat
{
public:
	
};

int main()
{

}
