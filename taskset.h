#ifndef _TASKSET_H_
#define _TASKSET_H_

#include <fstream>
#include <stdlib.h>
#include <iostream>

class taskset {
	public:
		taskset(std::ifstream &fs);
		~taskset();

		void output();

		int tnum;
		int pnum;
		int *period;
		int *execute;
		int lcm_period;
		int rnum;
		int *rwidth;
		int *b;

	private:
		void getRNum_();
};

#endif
