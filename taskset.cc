
#include "taskset.h"

static void _swap(int &a, int &b) {
	int tmp;
	tmp=a;
	a=b;
	b=tmp;
}

static int _gcd(int a, int b) {
	if(0==a)
		return b;
	if(0==b)
		return a;
	if(a>b)
		_swap(a, b);

	int c;
	for(c=a%b; c>0; c=a%b)
	{
		a=b;
		b=c;
	}
	return b;
}

static int _lcm(int a, int b) {
	if(0==a)
		return b;
	if(0==b)
		return a;

	int gcd=_gcd(a, b);
	if(gcd==0)
		return 0;

	int res=a/gcd;
	res=res*b;

	return res;
}

void taskset::getRNum_() {
	bool *flag=(bool *)malloc(sizeof(bool)*lcm_period);
	for(int i=0; i<lcm_period; i++)
		flag[i]=false;
	for(int i=0; i<tnum; i++)
		for(int j=0; j<lcm_period/period[i]; j++)
			flag[j*period[i]]=true;
	rnum=0;
	for(int i=0; i<lcm_period; i++)
		if(flag[i])
			rnum++;

	rwidth=(int *)malloc(sizeof(int)*rnum);
	b=(int *)malloc(sizeof(int)*(rnum+1));
	b[rnum]=lcm_period;
	int cur=lcm_period;
	int ind=rnum-1;
	for(int i=lcm_period; i>=0; i--)
	{
		if(flag[i])
		{
			rwidth[ind]=cur-i;
			b[ind]=i;
			cur=i;
			ind--;
		}
	}
}

taskset::taskset(std::ifstream &fs) {
	fs>>tnum>>pnum>>lcm_period;
	period=(int *)malloc(sizeof(int)*tnum);
	execute=(int *)malloc(sizeof(int)*tnum);
	for(int i=0; i<tnum; i++)
		fs>>period[i];
	for(int i=0; i<tnum; i++)
		fs>>execute[i];
	getRNum_();
}

taskset::~taskset() {
	free(period);
	free(execute);
	free(rwidth);
	free(b);
}

void taskset::output() {
	std::cout<<tnum<<'\t'<<pnum<<'\t'<<lcm_period<<std::endl;
	for(int i=0; i<tnum; i++)
		std::cout<<period[i]<<'\t';
	std::cout<<std::endl;
	for(int i=0; i<tnum; i++)
		std::cout<<execute[i]<<'\t';
	std::cout<<std::endl;

	std::cout<<rnum<<std::endl;
	for(int i=0; i<rnum; i++)
		std::cout<<rwidth[i]<<'\t';
	std::cout<<std::endl;
}

