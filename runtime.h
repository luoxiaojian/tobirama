#ifndef _RUNTIME_H_
#define _RUNTIME_H_

#include "taskset.h"
#include "fraction.h"

#include <vector>
#include <algorithm>
#include <iostream>

#define MIN(a, b) ((a>b)?b:a)

using std::vector;
using std::cout;
using std::endl;

class taskstate {
public:
	explicit taskstate(int, int, int, int);

	int tid;
	fraction *al;
	fraction ret;
	fraction deadline;
};

class job {
public:
	explicit job(int, const fraction&, const fraction&);

	int tid;
	fraction dur;
	fraction start, end;
};

class runtime {
public:
	explicit runtime(const taskset& );

	void updateUMat();
	void updateAL(int cur);
	void fakeAlloc(const fraction& cap);
	void alloc(int start, int end);
	void updateState(int cur, int next);

	vector<taskstate> tslist;
	fraction **umat;
	vector<job> *allocation;

	fraction *ui;

	void outputAlloc() const;

private:
	void initTSlist(const taskset&);
	void sortTSlist();

	fraction bdg(int tid, int pid, int tend);

	int tnum, pnum;
	int *execute, *period;
};

#endif
