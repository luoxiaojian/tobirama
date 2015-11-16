#include "runtime.h"

taskstate::taskstate(int t, int pnum, int period, int execute) : tid(t), al(new fraction[pnum]), ret(execute), deadline(period) {
	for(int i=0; i<pnum; i++)
		al[i]=0;
}

job::job(int t, const fraction& s, const fraction& e) {
	tid=t;
	start=s;
	end=e;
	dur=end-start;
}

void runtime::initTSlist(const taskset& ts) {
	int pnum=ts.pnum;
	int tnum=ts.tnum;
	for(int i=0; i<tnum; i++)
		tslist.push_back(taskstate(i, pnum, ts.period[i], ts.execute[i]));
}

runtime::runtime(const taskset& ts) : tnum(ts.tnum), pnum(ts.pnum), execute(ts.execute), period(ts.period){
	initTSlist(ts);
	umat=(fraction**)malloc(sizeof(fraction*)*tnum);
	for(int i=0; i<tnum; i++)
		umat[i]=new fraction[pnum];
	ui=new fraction[tnum];
	for(int i=0; i<tnum; i++)
		ui[i]=fraction(ts.execute[i], ts.period[i]);
	allocation=new vector<job>[pnum];
}

static bool cmpByDeadline(const taskstate& t1, const taskstate& t2) {
	return (t1.deadline<t2.deadline);
}

void runtime::sortTSlist() {
	sort(tslist.begin(), tslist.end(), cmpByDeadline);
}

void runtime::updateUMat() {
	for(int i=0; i<tnum; i++)
		for(int j=0; j<pnum; j++)
			umat[i][j]=0;
	sortTSlist();
	fraction acc=0;
	int cur=0;
	for(vector<taskstate>::iterator it=tslist.begin(); it!=tslist.end(); it++) {
		int tid=it->tid;
		fraction start=acc;
		fraction end=start+ui[tid];
		if(end>1) {
			umat[tid][cur]=1-start;
			umat[tid][cur+1]=end-1;
			cur++;
			acc=end-1;
		} else {
			umat[tid][cur]=ui[tid];
			acc=end;
		}
/*		cout<<tid<<": ";
		for(int i=0; i<pnum; i++)
			cout<<umat[tid][i].value()<<" | ";
		cout<<endl;*/
	}
}

fraction runtime::bdg(int tid, int pid, int tend) {
	fraction res;
	vector<taskstate>::iterator it;
	for(it=tslist.begin(); it!=tslist.end(); it++)
	{
		if(it->tid==tid) {
			res+=it->al[pid];
			res+=(umat[tid][pid]*(tend-it->deadline));
			break;
		}
	}
	return res;
}

void runtime::updateAL(int cur) {
	vector<int> hp;
	for(vector<taskstate>::iterator it=tslist.begin(); it!=tslist.end(); it++) {
		int tid=it->tid;
		fraction pacc;
		for(int i=0; i<pnum; i++) {
			fraction almax=it->deadline-cur;
			for(vector<int>::iterator iit=hp.begin(); iit!=hp.end(); iit++)
				almax-=bdg(*iit, i, it->deadline.value());
			almax-=pacc;
			it->al[i]=MIN(almax, it->ret-pacc);
			pacc+=it->al[i];
		}
		hp.push_back(tid);
//		cout<<tid<<": ";
//		for(int i=0; i<pnum; i++)
//			cout<<it->al[i].value()<<" | ";
//		cout<<endl;
	}
}

void runtime::fakeAlloc(const fraction& cap) {
	fraction *remains=new fraction[pnum];
	for(int i=0; i<pnum; i++)
		remains[i]=cap;
	for(vector<taskstate>::iterator it=tslist.begin(); it!=tslist.end(); it++) {
		for(int i=0; i<pnum; i++)
		{
			fraction all=MIN(remains[i], it->al[i]);
			if(all>0)
			{
				remains[i]-=all;
				it->al[i]-=all;
				it->ret-=all;
			}
		}
	}
}

void runtime::alloc(int start, int end) {
	fraction *cur=new fraction[pnum];
	fraction *cap=new fraction[pnum];
	for(int i=0; i<pnum; i++)
	{
		cur[i]=start;
		cap[i]=end-start;
	}
	for(vector<taskstate>::iterator it=tslist.begin(); it!=tslist.end(); it++) {
		for(int i=0; i<pnum; i++)
		{
			fraction all=MIN(cap[i], it->al[i]);
			if(all>0) {
				cap[i]-=all;
				it->al[i]-=all;
				it->ret-=all;
				allocation[i].push_back(job(it->tid, cur[i], cur[i]+all));
				cur[i]+=all;
			}
		}
	}

}

void runtime::updateState(int cur, int next) {
	for(vector<taskstate>::iterator it=tslist.begin(); it!=tslist.end(); it++) {
		if(it->deadline==next) {
			if(!(it->ret==0))
				cout<<"!!!!!"<<it->tid<<"alloc error"<<endl;
	//		else
	//			cout<<"---->task "<<it->tid<<" meet its deadline"<<endl;
			it->ret=execute[it->tid];
			it->deadline+=period[it->tid];
		}
	}
}

void runtime::outputAlloc() const {
	for(int i=0; i<pnum; i++)
	{
		cout<<"p"<<i<<": |";
		for(vector<job>::iterator it=allocation[i].begin(); it!=allocation[i].end(); it++)
			cout<<" "<<it->tid<<", "<<it->dur.strval()<<" |";
		cout<<endl;
	}
}
