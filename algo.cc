#include <iostream>
#include <fstream>

#include "taskset.h"
#include "runtime.h"

using namespace std;

int rnum, tnum, pnum, lcm_period;
int *execute, *period, *rwidth, *b;

void allocARow(int rid) {

}

int main()
{
	ifstream fin("bf01.cfg");
	taskset ts(fin);

	rnum=ts.rnum;
	tnum=ts.tnum;
	pnum=ts.pnum;
	lcm_period=ts.lcm_period;
	execute=ts.execute;
	period=ts.period;
	rwidth=ts.rwidth;
	b=ts.b;

	runtime rt(ts);

	for(int i=0; i<rnum; i++)
	{
		rt.updateUMat();
		rt.updateAL(b[i]);
		rt.alloc(b[i], b[i+1]);
		rt.updateState(b[i], b[i+1]);
	}

	rt.outputAlloc();

	return 0;

}
