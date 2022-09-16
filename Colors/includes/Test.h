#ifndef Test_H
#define Test_H

#include <iostream>

class Test
{
public:
	int x,y,z = 0;
	Test(int _x = 0,int _y=0,int _z=0)
	{
		x = _x;
		y = _y;
		z = _z;
	};

	void print() {
		std::cout << x << "\t" << y << "\t" << z << std::endl;
	}
	
};

#endif