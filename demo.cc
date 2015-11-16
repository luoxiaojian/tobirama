#include <iostream>
#include "fraction.h"

using namespace std;

int main()
{
	fraction a(3, 2);
	fraction b(1, 3);

	fraction c(a);
	c.output();
	cout<<c.value()<<endl;
	return 0;
}
